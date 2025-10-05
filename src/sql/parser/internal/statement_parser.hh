#ifndef _SQL_PARSER_INTERNAL_STATEMENT_PARSER_HH
#define _SQL_PARSER_INTERNAL_STATEMENT_PARSER_HH

#include "reader.hh"
#include "statement.hh"

namespace sql::parser::detail::statement::parser {
  sql::parser::statement::select parse_select(detail::token_reader__& reader);
  sql::parser::statement::where parse_where(detail::token_reader__& reader);
}

#endif /* _SQL_PARSER_INTERNAL_STATEMENT_PARSER_HH */