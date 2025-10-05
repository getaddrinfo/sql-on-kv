#ifndef _SQL_LEXER_INTERNAL_CHARSET_HH
#define _SQL_LEXER_INTERNAL_CHARSET_HH

namespace sql::lexer::detail {
  /**
   * Returns true if the character is a valid character of an
   * identity token.
   */
  bool is_ident(char);

  /**
   * Returns true if the character is a valid integer value.
   */
  bool is_integer(char);

  /**
   * Returns true if the value is counted as a quote character.
   */
  bool is_quote(char);
}

#endif /* _SQL_LEXER_INTERNAL_CHARSET_HH */