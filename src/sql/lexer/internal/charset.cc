#include "charset.hh"

namespace sql::lexer::detail {
  bool is_ident(char c) {
    if (c >= 'a' && c <= 'z') {
      return true;
    }

    if (c >= 'A' && c <= 'Z') {
      return true;
    }

    if (c == '_') {
      return true;
    }

    return false;
  }
  
  bool is_integer(char c) {
    return c >= '0' && c <= '9';
  }

  bool is_quote(char c) {
    return c == '"';
  }
}