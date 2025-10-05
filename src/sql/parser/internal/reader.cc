#include "reader.hh"

#include <strstream>

#include "absl/log/check.h"

namespace sql::parser::detail {
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
      << token().literal
      << ')';
  }

  void token_reader__::must_one_of(
    std::initializer_list<sql::lexer::TokenType> tys
  ) {
    for(const sql::lexer::TokenType ty : tys) {
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

    stream << '}';

    CHECK(false) << "Expected token to be one of "
      << stream.str()
      << " but got " << token().type_name()
      << '('
      << token().literal
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

      if (token.ty != sql::lexer::TokenType::Space) {
        break;
      }

      reader.next();
    }
  }
}