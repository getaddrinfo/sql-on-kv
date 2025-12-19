#ifndef _SQL_PARSER_INTERNAL_WHERE_PARSER_HH
#define _SQL_PARSER_INTERNAL_WHERE_PARSER_HH

#include "internal/reader.hh"
#include "statement.hh"

namespace sql::parser::statement::detail::where {
  using sql::parser::detail::TokenReader;


  /**
   * Parses a where statement from a `token_reader__`.
   */
  std::shared_ptr<sql::parser::statement::Where> parse(TokenReader& reader);
}

#endif /* _SQL_PARSER_INTERNAL_WHERE_PARSER_HH */