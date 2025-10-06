#ifndef _SQL_LEXER_TOKEN_HH
#define _SQL_LEXER_TOKEN_HH

#include <string_view>

namespace sql::lexer {
  /**
   * Represents the type that the token is.
   */
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


  /**
   * Represents an instance of a type of token,
   * with a view to the data that comprises this token,
   * and the type it is.
   */
  struct Token {
  private:
    const Type _type;
    const std::string_view _literal;
    
  public:
    Token(
      const Type type,
      const std::string_view literal
    ) : _type(type), _literal(literal) {}

    /**
     * Gets the type of this token.
     */
    const Type type() const;

    /**
     * Gets a view to the contents of this token.
     */
    const std::string_view literal() const;

    /**
     * Gets a string representation of the token.
     */
    const std::string repr() const;

    /**
     * Gets a string representation of the name of this token.
     */
    const std::string& type_name() const;

    /**
     * Gets a string representation of the name of the given type.
     */
    static const std::string& type_name(const Type type);
  };

}

#endif /* _SQL_LEXER_TOKEN_HH */