#include "sql/parser.hh"

#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "absl/log/log.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"

namespace sql::parser {
  using sql::lexer::TokenType;

  using ParserFn = std::function<
    ParseResult (detail::token_reader__&)
  >;

  static std::unordered_map<TokenType, ParserFn> _dispatcher = {
    {TokenType::Select, [](detail::token_reader__& reader) {
      return ParseResult(detail::parse_select(reader));
    }},
  };

  ParseResult parse(
    const std::vector<sql::lexer::Token>& tokens
  ) {
    detail::token_reader__ reader(tokens);
    detail::trim_left(reader);

    if (reader.empty()) {
      // TODO: this
    }

    const sql::lexer::Token& token = reader.token();

    bool found = false;
    for(const auto& dispatchable : _dispatcher) {
      found = found || reader.is(dispatchable.first);
    }

    QCHECK(found) << "Not a valid token to start: "
      << reader.token().type_name();

    reader.back();

    ParserFn& parser = _dispatcher.at(token.ty);
    return parser(reader);
  }

  namespace detail {
    bool token_reader__::empty() const {
      return _index + 1 == _tokens.size();
    }

    bool token_reader__::is(sql::lexer::TokenType ty) {
      return token().ty == ty;
    }

    bool token_reader__::is_one_of(std::initializer_list<sql::lexer::TokenType> tys) {
      for(const sql::lexer::TokenType ty : tys) {
        if (is(ty)) {
          return true;
        }
      }

      return false;
    }

    void token_reader__::next() {
      if (empty()) {
        return;
      }

      _index++;
    }

    void token_reader__::back() {
      if (_index == 0) {
        return;
      }

      _index--;
    }

    void token_reader__::must(sql::lexer::TokenType ty) {
      CHECK(is(ty)) << "Token must be of type "
        << sql::lexer::token_type_name(ty) 
        << " but is of type "
        << sql::lexer::token_type_name(token().ty)
        << '('
        << token().literal.value_or("<no literal>")
        << ')';
    }

    void token_reader__::must_one_of(
      std::initializer_list<TokenType> tys
    ) {
      for(const TokenType ty : tys) {
        if (is(ty)) {
          return;
        }
      }

      std::ostringstream stream;
      stream << '{';

      for(auto it = tys.begin(); it != tys.end(); ++it) {
        stream << '"';
        stream << sql::lexer::token_type_name(*it);
        stream << '"';

        if (std::next(it) != tys.end()) {
          stream << ", ";
        }
      }

      stream << "} ";

      CHECK(false) << "Expected token to be one of "
        << stream.str()
        << " but got " << token().type_name()
        << '('
        << token().literal.value_or("<no literal>")
        << ')';
    }

    const sql::lexer::Token& token_reader__::token() const {
      return _tokens.at(_index);
    }

    void token_reader__::next_non_space() {
      detail::trim_left(*this);
    }

    void trim_left(token_reader__& reader) {
      while (!reader.empty()) {
        const sql::lexer::Token& token = reader.token();

        if (token.ty != TokenType::Space) {
          break;
        }

        reader.next();
      }
    }

    statement::select parse_select(token_reader__& reader) {
      std::vector<std::string> fields;

      reader.must(TokenType::Select);
      reader.next();
      
      detail::trim_left(reader);

      // We are reading in the fields for `select`.
      do {
        // Move to the next non-space token

        // We must have a string (ignoring the fact that
        // functions can be used in these places)
        reader.must(TokenType::Ident);

        // Consume the token, and add the fields to the strings
        const sql::lexer::Token& str = reader.token();
        fields.push_back(std::string(str.literal.value()));
        
        reader.next();
        
        if (reader.is(TokenType::Comma)) {
          reader.next();
        }

        detail::trim_left(reader);
      } while (!reader.is(TokenType::From) && !reader.is(TokenType::End));

      for(const std::string& field : fields) {
        LOG(INFO) << "- " << field;
      }

      reader.must(TokenType::From);
      reader.next();
      detail::trim_left(reader);

      reader.must(TokenType::Ident);
      std::string table(
        reader.token().literal.value()
      );

      reader.next();
      detail::trim_left(reader);

      LOG(INFO) << "Table: " << table;

      parse_where(reader);

      return statement::select{};
    }

    std::unique_ptr<statement::where> _do_parse_comparison(
      token_reader__& reader
    ) {
      reader.must(TokenType::Ident);

      std::string field(
        reader.token().literal.value()
      );

      reader.next();
      detail::trim_left(reader);

      reader.must_one_of({
        TokenType::Equals,
        TokenType::In
      });

      TokenType cmp = reader.token().ty;

      reader.next();
      detail::trim_left(reader);

      // Must be a string or an integer.
      reader.must_one_of({
        TokenType::String,
        TokenType::Integer
      });

      // TODO: Switch on this
      // For now, assume it's a string.

      std::string value(
        reader.token().literal.value()
      );
    

      return std::make_unique<statement::comparison>(
        field,
        statement::Operator::Equals,
        value
      );
    }

    static std::unordered_set<TokenType> _junction_ops = {
      TokenType::And,
      TokenType::Or
    };

    std::unique_ptr<statement::where> _do_parse_group(
      token_reader__& reader,
      size_t depth
    ) {
      std::vector<std::unique_ptr<statement::where>> parts;

      if (reader.is(TokenType::RightParen)) {
        reader.next();


        // Parse this inner group
        parts.push_back(_do_parse_group(
          reader,
          depth + 1
        ));
      }

      // TODO: We need to understand the conj/disj between statements properly.
      do {
        parts.push_back(_do_parse_comparison(reader));


        reader.next();
        detail::trim_left(reader);

        // If we have a statement that is `AND` or `OR`, we continue parsing.
        //
        // TODO: This will fail if the next token is END, which is valid.
        reader.must_one_of({
          TokenType::And,
          TokenType::Or,
          TokenType::LeftParen
        });

        if (_junction_ops.contains(reader.token().ty)) {
          LOG(INFO) << "junction";
          reader.next();
          detail::trim_left(reader);
        }
      } while (!reader.is(TokenType::LeftParen));

      return std::make_unique<statement::condition>();
    }

    statement::where parse_where(
      token_reader__& reader
    ) {
      // We must be in a WHERE clause if we are in this method.
      reader.must(TokenType::Where);
      reader.next();
      detail::trim_left(reader);

      if (reader.is(TokenType::RightParen)) {
        reader.next();
      }

      // We need to stitch together each group, since it could be
      do {
        std::unique_ptr<statement::where> cond = _do_parse_group(
          reader,
          1
        );

        reader.next();
        detail::trim_left(reader);
      } while (reader.is_one_of({
        TokenType::And,
        TokenType::Or
      }));

      /*
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

      return statement::where{};
    }
  }
}