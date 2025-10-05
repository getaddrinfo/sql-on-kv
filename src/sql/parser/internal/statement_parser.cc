#include "statement_parser.hh"
#include "reader.hh"
#include "statement.hh"

#include "absl/log/check.h"
#include "absl/log/log.h"

#include <unordered_set>

namespace sql::parser::detail::statement::parser {
  using lexer::Token;
  using lexer::Type;

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
    } while (!reader.is(Type::From) && !reader.is(Type::End));

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
      sql::parser::statement::Operator::Equals,
      value
    );
  }

  /**
   * The set of operations that count as junctions of
   * two other statements (AND, OR).
   */
  static std::unordered_set<Type> _junction_ops = {
    Type::And,
    Type::Or
  };

  std::unique_ptr<sql::parser::statement::where> _do_parse_group(
    token_reader__& reader,
    size_t depth
  ) {
    // TODO: Clean this up so that we know precedence of the parts.
    std::vector<std::unique_ptr<sql::parser::statement::where>> parts;

    do {
      if (reader.is(Type::RightParen)) {
        reader.next();

        // Parse this inner group
        parts.push_back(_do_parse_group(
          reader,
          depth + 1
        ));
      } else {
        parts.push_back(_do_parse_comparison(reader));
      }

      reader.next();
      detail::trim_left(reader);

      // Each of these are valid:
      // - AND, OR: Continuation of current group
      // - LeftParen: Closing of current group
      // - RightParen: Opening of a new group
      // - End: End of the input.
      reader.must_one_of({
        Type::And,
        Type::Or,
        Type::LeftParen,
        Type::RightParen,
        Type::End
      });

      if (reader.is(Type::End)) {
        break;
      }

      // TODO: Consume the tokens and group them properly
      if (_junction_ops.contains(reader.token().type())) {
        reader.next();
        detail::trim_left(reader);
      }
    } while (!reader.is(Type::LeftParen));

    reader.next();
    detail::trim_left(reader);

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
    - Augmenting brackets if required (simple)
    - Parsing statements at the bracket level (simple)
    - Inserting brackets where required to respect precedence (AND > OR > ...)
    - Parsing these into a class or tree that represents these conditions and can 
      easily be parsed at the key-value layer.
    */


    // We must be in a WHERE clause if we are in this method.
    reader.must(Type::Where);
    reader.next();
    detail::trim_left(reader);

    if (reader.is(Type::RightParen)) {
      reader.next();
    }

    // We need to stitch together each group, since it could be
    do {
      std::unique_ptr<sql::parser::statement::where> cond = _do_parse_group(
        reader,
        1
      );

      reader.next();
      detail::trim_left(reader);
    } while (reader.is_one_of({
      Type::And,
      Type::Or
    }));

    return sql::parser::statement::where{};
  }
}