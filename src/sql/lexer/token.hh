#ifndef _SQL_LEXER_TOKEN_HH
#define _SQL_LEXER_TOKEN_HH

#include <string_view>

namespace sql::lexer {
  enum class Type {
    Error,
    Asterisk,
    Comma,
    Space,
    Ident,
    Equals,
    Assign,
    Delim,
    LeftParen,
    RightParen,
    TemplatePlaceholder,
    Integer,
    String,
    And,
    Or,
    Insert,
    In,
    Into,
    Select,
    Delete,
    From,
    Where,
    Limit,
    Values,
    Update,
    Set,
    Create,
    Drop,
    Table,
    Semicolon,
    End
  };


  struct Token {
  private:
    const Type _type;
    const std::string_view _literal;
    
  public:
    Token(
      const Type type,
      const std::string_view literal
    ) : _type(type), _literal(literal) {}

    const Type type() const;
    const std::string_view literal() const;
    const std::string& type_name() const;

    static const std::string& type_name(const Type type);
  };

}

#endif /* _SQL_LEXER_TOKEN_HH */