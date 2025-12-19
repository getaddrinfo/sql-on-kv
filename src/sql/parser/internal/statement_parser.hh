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
  sql::parser::statement::Select parse_select(detail::TokenReader& reader);

  /**
   * Parses a `WHERE` clause from ` token reader.
   * 
   * @throws
   */
  // sql::parser::statement::Where parse_where(detail::TokenReader& reader);
}

#endif /* _SQL_PARSER_INTERNAL_STATEMENT_PARSER_HH */