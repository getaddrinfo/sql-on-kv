#include "reader.hh"

#include <strstream>

#include "absl/log/check.h"
#include <iterator>

namespace sql::parser::detail {
  bool token_reader__::empty() const {
    // If we are at the last slot, then our index is
    // one less than the number of tokens.
    return _index + 1 == _tokens.size();
  }

  bool token_reader__::is(sql::lexer::Type ty) {
    return token().type() == ty;
  }

  bool token_reader__::is_one_of(std::initializer_list<sql::lexer::Type> tys) {
    for(const sql::lexer::Type ty : tys) {
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

  void token_reader__::must(sql::lexer::Type ty) {
    CHECK(is(ty)) << "Token must be of type "
      << sql::lexer::Token::type_name(ty) 
      << " but is of type "
      << token().type_name()
      << '('
      << token().literal()
      << ')';
  }

  void token_reader__::must_one_of(
    std::initializer_list<sql::lexer::Type> tys
  ) {
    // Instead of doing CHECK(is_one_of(tys)), we 
    // return early to save any expensive computation to produce the stream
    // that represents the number of tokens.
    //
    // TODO: If we used variadic template args, maybe we could produce a constexpr
    // version of the pretty printed types.
    if (is_one_of(tys)) {
      return;
    }

    // Produce a pretty version of the names of tokens.
    // TODO: Move this to its own function?
    std::ostringstream stream;
    stream << '{';

    for(auto it = tys.begin(); it != tys.end(); ++it) {
      stream << '"';
      stream << sql::lexer::Token::type_name(*it);
      stream << '"';

      if (std::next(it) != tys.end()) {
        stream << ", ";
      }
    }

    stream << '}';

    // Follows from `is_one_of(tys)` not resulting in an early bail.
    CHECK(false) << "Expected token to be one of "
      << stream.str()
      << " but got " << token().type_name()
      << '('
      << token().literal()
      << ')';
  }

  const sql::lexer::Token& token_reader__::token() const {
    CHECK(!empty()) << "fatal: reader is empty";

    return _tokens.at(_index);
  }

  void token_reader__::next_non_space() {
    detail::trim_left(*this);
  }

  void trim_left(token_reader__& reader) {
    while (!reader.empty()) {
      const sql::lexer::Token& token = reader.token();

      if (token.type() != sql::lexer::Type::Space) {
        break;
      }

      reader.next();
    }
  }
}