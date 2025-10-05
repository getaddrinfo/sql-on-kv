#ifndef _SQL_PARSER_INTERNAL_STATEMENT_PARSER_HH
#define _SQL_PARSER_INTERNAL_STATEMENT_PARSER_HH

#include "reader.hh"
#include "statement.hh"

namespace sql::parser::detail::statement::parser {
  /**
   * Parses a `SELECT` statement from a token reader.
   * 
   * @throws
   */
  sql::parser::statement::select parse_select(detail::token_reader__& reader);

  /**
   * Parses a `WHERE` clause from ` token reader.
   * 
   * @throws
   */
  sql::parser::statement::where parse_where(detail::token_reader__& reader);
}

#endif /* _SQL_PARSER_INTERNAL_STATEMENT_PARSER_HH */