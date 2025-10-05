#ifndef _SQL_LEXER_HH
#define _SQL_LEXER_HH

#include <vector> 
#include <optional>
#include <string_view>

#include "token.hh"

namespace sql::lexer {
  using Tokens = std::vector<Token>;

  Tokens lex(const std::string& source);
}

#endif /* _SQL_LEXER_HH */