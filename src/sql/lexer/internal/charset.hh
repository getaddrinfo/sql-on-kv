#ifndef _SQL_LEXER_INTERNAL_CHARSET_HH
#define _SQL_LEXER_INTERNAL_CHARSET_HH

namespace sql::lexer::detail {
  bool is_ident(char);
  bool is_integer(char);
  bool is_quote(char);
}

#endif /* _SQL_LEXER_INTERNAL_CHARSET_HH */