#ifndef _SQL_LEXER_HH
#define _SQL_LEXER_HH

#include <vector> 
#include <optional>
#include <string_view>

namespace sql::lexer {
  enum class TokenType {
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

  const std::string& token_type_name(const TokenType ty);

  struct Token {
    TokenType ty;
    std::optional<const std::string_view> literal;

    const std::string& type_name() const;
  };




  std::vector<Token> lex(const std::string& source);

  namespace detail {
    bool is_quote(char);
    bool is_integer(char);
    bool is_space(char);
    bool is_open_bracket(char);
    bool is_close_bracket(char);
    bool is_semicolon(char);
    bool is_asterisk(char);
    bool is_comma(char);
    bool is_equals(char);
    bool is_ident(char);

    class consumption_state__ {
    private:
      const std::string_view _data;
      size_t _curr_index;

    public:
      consumption_state__(const std::string_view data)
        : _data(data), _curr_index(0) {}

      bool next();
      void back();

      Token consume();
      const char current() const;
      const size_t index() const;
      const std::string_view data() const;
      const bool finished() const;
    };

    class ident_consumer__ {
    private:
      consumption_state__& _parent;
    
    public:
      ident_consumer__(
        consumption_state__& parent
      ) : _parent(parent) {}

      Token consume();
    };

    class string_consumer__ {
    private:
      consumption_state__& _parent;
    public:
      string_consumer__(
        consumption_state__& parent
      ) : _parent(parent) {}

      Token consume();
    };

    class int_consumer__ {
    private:
      consumption_state__& _parent;
    public:
      int_consumer__(
        consumption_state__& parent
      ) : _parent(parent) {}

      Token consume();
    };
  }
}

#endif /* _SQL_LEXER_HH */