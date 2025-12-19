#include "parser.hh"

#include "shunt.hh"

#include "absl/log/log.h"

namespace sql::parser::statement::detail::where {
  using sql::lexer::Type;
  using sql::parser::detail::trim_left;

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

  std::shared_ptr<sql::parser::statement::Where> _do_parse_comparison(
    TokenReader& reader
  ) {
    // Must consist of IDENT COMPARISON VALUE
    reader.must(Type::Ident);

    std::string field(reader.token().literal());

    reader.next();
    trim_left(reader);

    // Either we are doing a comparison, or we are checking for
    // presence.
    reader.must_one_of({
      Type::Equals,
      Type::In
    });

    Type cmp = reader.token().type();
    CHECK(_operator_mapping.contains(cmp)) << "Cannot find operator in permitted operators of comparison.";

    sql::parser::statement::Operator op = _operator_mapping.at(cmp);

    reader.next();
    trim_left(reader);

    // Must be a string, an integer, or an identity.
    reader.must_one_of({
      Type::String,
      Type::Integer,
      Type::Ident
    });

    // TODO: Switch on the type to handle this differently
    // For now, assume it's a string.
    std::string value(reader.token().literal());
  
    return std::make_shared<sql::parser::statement::Comparison>(
      field,
      op,
      value
    );
  }

  std::string _pretty_print_where(
    std::shared_ptr<sql::parser::statement::Where> node
  ) {
    std::shared_ptr<sql::parser::statement::Comparison> comp 
      = std::dynamic_pointer_cast<sql::parser::statement::Comparison>(node);

    std::shared_ptr<sql::parser::statement::Condition> cond
      = std::dynamic_pointer_cast<sql::parser::statement::Condition>(node);

    CHECK(comp || cond) << "Must be one of (comparison, condition)";

    if (cond) {
      return absl::StrFormat(
        "Comparison(op=%s, lhs=%s, rhs=%s)",
        sql::parser::statement::operator_name(cond->op()),
        _pretty_print_where(cond->lhs()),
        _pretty_print_where(cond->rhs())
      );
    }

    if (comp) {
      return absl::StrFormat(
        "Condition(field=%s, op=%s, value=%s)",
        comp->column(),
        sql::parser::statement::operator_name(comp->op()),
        comp->value()
      );
    }

    CHECK(false) << "unreachable";
  }

  std::shared_ptr<sql::parser::statement::Where> _do_parse_group(
    TokenReader& reader
  ) {
    std::vector<std::shared_ptr<sql::parser::statement::Where>> conditions;
    std::vector<sql::parser::statement::Operator> operators;

    // Cases:
    // - This function has been called at the start of a group `(...`
    // - This function has been called at the end of a group `...)

    comp: switch (reader.token().type()) {
      case Type::RightParen: {
        LOG(INFO) << reader.highlight_current_token() << '\n';
        reader.next();
        conditions.push_back(_do_parse_group(reader));

        goto comp;
      }

      case Type::Ident: {
        LOG(INFO) << reader.highlight_current_token();
        conditions.push_back(_do_parse_comparison(reader));
        reader.next();

        goto comp;
      }

      case Type::Space: {
        trim_left(reader);

        goto comp;
      }

      case Type::And:
      case Type::Or: {
        LOG(INFO) << reader.highlight_current_token() << '\n';
        CHECK(_junction_mapping.contains(reader.token().type()))
          << "Cannot lookup expected token in WHERE clause in junction map";
        operators.push_back(
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
    trim_left(reader);

    CHECK_EQ(operators.size(), conditions.size() - 1)
      << "Mismatch in operators and conditions";


    // SAFETY: junctions.size() == conditions.size() - 1
    // Hence, conditions must have one element if junctions has 0.
    if (operators.size() == 0) {
      return conditions.at(0);
    }
    
    std::shared_ptr<sql::parser::statement::Where> result = ::detail::shunt(
      conditions,
      operators
    );

    VLOG(1) << "Shunted result: " << _pretty_print_where(result);

    return result;
  }

  std::shared_ptr<sql::parser::statement::Where> parse(TokenReader& reader) {
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

    reader.must(Type::Where);
    reader.next();
    sql::parser::detail::trim_left(reader);

    return _do_parse_group(reader);
  }
}