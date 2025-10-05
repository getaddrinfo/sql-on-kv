#ifndef _SQL_LEXER_HH
#define _SQL_LEXER_HH

#include <vector> 
#include <optional>
#include <string_view>

#include "token.hh"

namespace sql::lexer {
  /**
   * The type used to represent tokens that the lexing process
   * produces.
   */
  using Tokens = std::vector<Token>;

  /**
   * Lex a string into tokens for SQL.
   */
  Tokens lex(const std::string& source);
}

#endif /* _SQL_LEXER_HH */