#include "statement_parser.hh"
#include "reader.hh"
#include "statement.hh"

#include "absl/log/check.h"
#include "absl/log/log.h"

#include <unordered_set>

namespace sql::parser::detail::statement::parser {
  using lexer::Token;
  using lexer::Type;


  struct compare_precedence {
    bool operator()(
      std::tuple<sql::parser::statement::Operator, size_t, size_t>& a,
      std::tuple<sql::parser::statement::Operator, size_t, size_t>& b
    ) const {
      return std::get<0>(a) > std::get<0>(b);
    }
  };


  sql::parser::statement::select parse_select(detail::token_reader__& reader) {
    std::vector<std::string> fields;

    reader.must(Type::Select);
    reader.next();
    
    detail::trim_left(reader);

    do {
      // We must have an ident (ignoring the fact that
      // functions can be used in these places)
      reader.must(Type::Ident);

      // Consume the token, and add the fields to the strings
      const sql::lexer::Token& str = reader.token();
      fields.push_back(std::string(str.literal()));
      
      reader.next();
      
      // Skip a comma so that we can check if there is another ident
      // token to treat as a field.
      if (reader.is(Type::Comma)) {
        reader.next();
      }

      detail::trim_left(reader);
    } while (!reader.is_one_of({Type::From, Type::End}));

    // TODO: Move to VLOG
    for(const std::string& field : fields) {
      LOG(INFO) << "- " << field;
    }

    // Next part must be a `FROM` statement.
    reader.must(Type::From);
    reader.next();
    detail::trim_left(reader);

    // Next part must be a table name.
    reader.must(Type::Ident);
    std::string table(
      reader.token().literal()
    );

    reader.next();
    detail::trim_left(reader);

    LOG(INFO) << "Table: " << table;

    // TODO: After this, most statements can vary in order, so we should instead use a
    // dispatcher map that parses on the token type.
    parse_where(reader);

    // TODO: Build the statement from the partial state produced by the parsers.
    return sql::parser::statement::select{};
  }

  static std::unordered_map<
    sql::lexer::Type,
    sql::parser::statement::Operator
  > _operator_mapping = {
    {sql::lexer::Type::Equals, sql::parser::statement::Operator::Equals},
  };

  static std::unordered_map<
    sql::lexer::Type,
    sql::parser::statement::Operator
  > _junction_mapping = {
    {sql::lexer::Type::And, sql::parser::statement::Operator::And},
    {sql::lexer::Type::Or, sql::parser::statement::Operator::Or}
  };

  std::unique_ptr<sql::parser::statement::where> _do_parse_comparison(
    token_reader__& reader
  ) {
    // Must consist of IDENT COMPARISON VALUE
    reader.must(Type::Ident);

    std::string field(reader.token().literal());

    reader.next();
    detail::trim_left(reader);

    // Either we are doing a comparison, or we are checking for
    // presence.
    reader.must_one_of({
      Type::Equals,
      Type::In
    });

    Type cmp = reader.token().type();
    CHECK(_operator_mapping.contains(cmp)) << "Invalid";

    sql::parser::statement::Operator op = _operator_mapping.at(cmp);

    reader.next();
    detail::trim_left(reader);

    // Must be a string, an integer, or an identity.
    reader.must_one_of({
      Type::String,
      Type::Integer,
      Type::Ident
    });

    // TODO: Switch on the type to handle this differently
    // For now, assume it's a string.
    std::string value(reader.token().literal());
  
    return std::make_unique<sql::parser::statement::comparison>(
      field,
      op,
      value
    );
  }

  std::unique_ptr<sql::parser::statement::where> _do_parse_group(
    token_reader__& reader,
    size_t depth
  ) {
    std::vector<std::unique_ptr<sql::parser::statement::where>> conditions;
    std::vector<sql::parser::statement::Operator> junctions;

    // Cases:
    // - This function has been called at the start of a group `(...`
    // - This function has been called at the end of a group `...)

    comp: switch (reader.token().type()) {
      case Type::RightParen: {
        LOG(INFO) << reader.highlight_current_token() << '\n';
        reader.next();
        conditions.push_back(_do_parse_group(reader, depth+1));

        goto comp;
      }

      case Type::Ident: {
        LOG(INFO) << reader.highlight_current_token();
        conditions.push_back(_do_parse_comparison(reader));
        reader.next();

        goto comp;
      }

      case Type::Space: {
        detail::trim_left(reader);

        goto comp;
      }

      case Type::And:
      case Type::Or: {
        LOG(INFO) << reader.highlight_current_token() << '\n';
        CHECK(_junction_mapping.contains(reader.token().type()))
          << "Cannot lookup expected token in WHERE clause in junction map";
        junctions.push_back(
          _junction_mapping.at(reader.token().type())
        );

        reader.next();
        goto comp;
      }

      case Type::End: {
        LOG(INFO) << reader.token().type_name();
        break;
      }

      case Type::LeftParen: {
        LOG(INFO) << reader.highlight_current_token() << '\n';
        break;
      }

      default: {
        break;
      }
    }

    reader.next();
    detail::trim_left(reader);

    CHECK_EQ(junctions.size(), conditions.size() - 1)
      << "Mismatch in junctions and conditions";

    LOG(INFO) << absl::StrFormat("junctions:%d conditions:%d", junctions.size(), conditions.size());

    // We now need to zip for precedence
    std::vector<std::tuple<sql::parser::statement::Operator, size_t, size_t>> 
      precedence(junctions.size());

    // Zip the `operator` and the two conditions grouped by the junction as a tuple of 3 elements.
    for(size_t i = 0; i < junctions.size(); i++) {
      precedence.at(i) = std::make_tuple(junctions.at(i), i, i + 1);
    }

    // Sort using a precedence sorter built for the pair.
    std::sort(precedence.begin(), precedence.end(), compare_precedence{});

    for(auto& p : precedence) {
      LOG(INFO) << "- "
        << sql::parser::statement::operator_name(std::get<0>(p))
        << '('
        << std::get<1>(p)
        << ','
        << std::get<2>(p)
        << ')';
    }

    return std::make_unique<sql::parser::statement::condition>();
  }

  sql::parser::statement::where parse_where(
    token_reader__& reader
  ) {
    /*
    TODO: Convert these cases into standalone tests that validate the parse tree.
    
    Cases:
    WHERE field=something;
    WHERE field=something AND another_field=something;
    WHERE (field=something) AND (another_field=something);
    WHERE (field=something AND another_field=something);
    WHERE (field=something AND another_field=that) OR field=something_else;
    WHERE ((field = something) AND another_field=that);
    ...

    And so on

    We can imagine that a statement has brackets if it doesn't.
    `WHERE field=something` -> `WHERE (field=something)`
    `WHERE field=something AND another_field=something` -> `WHERE (field=something AND another_field=something)`

    Then, any other statements can be well bracketed. We also need to consider precedence.

    Hence, we need to split this parsing of a where statement into the following steps:
    - Parsing statements at the bracket level (simple)
    - Inserting brackets where required to respect precedence (AND > OR > ...)
    - Parsing these into a class or tree that represents these conditions and can 
      easily be parsed at the key-value layer.
    */

    // We must be in a WHERE clause if we are in this method.
    reader.must(Type::Where);
    reader.next();
    detail::trim_left(reader);

    _do_parse_group(reader, 0);

    return sql::parser::statement::where{};
  }
}