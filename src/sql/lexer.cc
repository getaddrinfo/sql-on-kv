#include "lexer.hh"

#include <unordered_map>

#include "absl/log/log.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"

#include <string>
#include <string_view>

namespace sql::lexer {
  std::vector<Token> lex(const std::string& source) {
    std::vector<Token> tokens;
    std::string_view view(source);
    detail::consumption_state__ tokeniser(view);

    while (!tokeniser.finished()) {
      sql::lexer::Token token = tokeniser.consume();

      LOG(INFO) 
        << token.type_name()
        << '('
        << token.literal.value_or("")
        << ')';

      tokens.push_back(token);
    }

    tokens.emplace_back(sql::lexer::TokenType::End, std::nullopt);

    return tokens;
  }

  struct string_view_hash {
    using is_transparent = void;
    
    size_t operator()(std::string_view sv) const noexcept {
      return std::hash<std::string_view>{}(sv);
    }

    template<std::convertible_to<std::string_view> T>
    size_t operator()(T const& t) const noexcept {
      return std::hash<std::string_view>{}(std::string_view(t));
    }
  };

  struct string_view_eq {
    using is_transparent = void;
    
    template<std::convertible_to<std::string_view> L,
      std::convertible_to<std::string_view> R>
    bool operator()(L const& l, R const& r) const noexcept {
      return std::string_view(l) == std::string_view(r);
    }
  };

  static std::unordered_map<const std::string, TokenType, string_view_hash, string_view_eq> _keywords = {
    {"AND", TokenType::And},
    {"OR", TokenType::Or},
    {"IN", TokenType::In},
    {"INSERT", TokenType::Insert},
    {"INTO", TokenType::Into},
    {"SELECT", TokenType::Select},
    {"DELETE", TokenType::Delete},
    {"FROM", TokenType::From},
    {"WHERE", TokenType::Where},
    {"LIMIT", TokenType::Limit},
    {"VALUES", TokenType::Values},
    {"UPDATE", TokenType::Update},
    {"SET", TokenType::Set},
    {"CREATE", TokenType::Create},
    {"DROP", TokenType::Drop},
    {"TABLE", TokenType::Table}
  };

  const std::string& token_type_name(const TokenType ty) {
    static std::unordered_map<TokenType, const std::string> map = {
      {TokenType::Error, "Error"},
      {TokenType::Asterisk, "Asterisk"},
      {TokenType::Comma, "Comma"},
      {TokenType::Space, "Space"},
      {TokenType::Ident, "Ident"},
      {TokenType::Equals, "Equals"},
      {TokenType::Assign, "Assign"},
      {TokenType::Delim, "Delim"},
      {TokenType::LeftParen, "LeftParen"},
      {TokenType::RightParen, "RightParen"},
      {TokenType::TemplatePlaceholder, "TemplatePlaceholder"},
      {TokenType::Integer, "Integer"},
      {TokenType::String, "String"},
      {TokenType::And, "And"},
      {TokenType::Or, "Or"},
      {TokenType::Insert, "Insert"},
      {TokenType::Into, "Into"},
      {TokenType::Select, "Select"},
      {TokenType::Delete, "Delete"},
      {TokenType::From, "From"},
      {TokenType::Where, "Where"},
      {TokenType::Limit, "Limit"},
      {TokenType::Values, "Values"},
      {TokenType::Update, "Update"},
      {TokenType::Set, "Set"},
      {TokenType::Create, "Create"},
      {TokenType::Drop, "Drop"},
      {TokenType::Table, "Table"},
      {TokenType::Semicolon, "Semicolon"},
      {TokenType::End, "End"},
      {TokenType::In, "In"}
    };

    auto result = map.find(ty);
    CHECK(result != map.end()) << "Unknown token type: " << static_cast<int>(ty);

    return result->second;
  }

  const std::string& Token::type_name() const {
    return token_type_name(ty);
  }

  namespace detail {    
    Token consumption_state__::consume() {
      char curr = current();

      // TODO: Change these branches to a quick map read, since 
      // `literal` doesn't need to be specified.
      if (is_comma(curr)) {
        Token token {
          .ty = TokenType::Comma,
          .literal = std::nullopt
        };

        next();
        return token;
      }

      if (is_semicolon(curr)) {
        Token token {
          .ty = TokenType::Semicolon,
          .literal = std::nullopt
        };

        next();
        return token;
      }

      if (is_asterisk(curr)) {
        Token token {
          .ty = TokenType::Asterisk,
          .literal = std::nullopt
        };

        next();
        return token;
      }

      if (is_space(curr)) {
        Token token {
          .ty = TokenType::Space,
          .literal = std::nullopt
        };

        next();
        return token;
      }

      if (is_open_bracket(curr)) {
        Token token {
          .ty = TokenType::RightParen,
          .literal = std::nullopt
        };

        next();
        return token;
      }

      if (is_close_bracket(curr)) {
        Token token {
          .ty = TokenType::LeftParen,
          .literal = std::nullopt
        };

        next();
        return token;
      }
      
      if (is_equals(curr)) {
        Token token {
          .ty = TokenType::Equals,
          .literal = std::nullopt
        };
        
        next();
        return token;
      }

      // Must be a branch
      if (is_quote(curr)) {
        detail::string_consumer__ consumer(*this);
        return consumer.consume();
      }

      // Must be a branch
      if (is_integer(curr)) {
        detail::int_consumer__ consumer(*this);
        return consumer.consume();
      }

      // Must be a branch
      if (is_ident(curr)) {
        detail::ident_consumer__ consumer(*this);
        return consumer.consume();
      }

      CHECK(false) << absl::StrFormat(
        "unreachable: input \"%c\" unmatched",
        curr
      );
    }

    bool consumption_state__::next() {
      if (_curr_index + 1 == _data.size()) {
        return false;
      }
      
      _curr_index++;
      return true;
    }

    void consumption_state__::back() {
      if (_curr_index == 0) {
        return;
      }

      _curr_index--;
    }

    const char consumption_state__::current() const {
      CHECK(_curr_index < _data.size()) << "Out of bounds read: idx " 
        << _curr_index
        << " of "
        << _data.size()
        << " elements.";

      return _data.at(_curr_index);
    }

    const size_t consumption_state__::index() const {
      return _curr_index;
    }
    
    const bool consumption_state__::finished() const {
      return _curr_index + 1 == _data.size();
    }


    const std::string_view consumption_state__::data() const {
      return _data;
    }

    Token ident_consumer__::consume() {
      size_t offset = _parent.index();
      size_t size = 0;

      do {
        char c = _parent.current();

        if (!is_ident(c)) {
          break;
        }
        
        size++;

        const std::string_view current_word = _parent.data().substr(
          offset,
          size
        );

        if (_keywords.find(current_word) != _keywords.end()) {
          // Consume last token
          _parent.next();

          return Token{
            .ty = _keywords.find(current_word)->second,
            .literal = current_word
          };
        }
      } while (_parent.next());

      return Token{
        .ty = TokenType::Ident,
        .literal = _parent.data().substr(
          offset,
          size
        )
      };
    }

    Token string_consumer__::consume() {
      size_t offset = _parent.index();
      size_t size = 0;

      char prev;

      do {
        char c = _parent.current();

        if (size > 0 && is_quote(c) && prev != '\\') {
          break;
        }

        size++;
        prev = c;
      } while (_parent.next());

      // Skip the last quote.
      _parent.next();

      return Token{
        .ty = TokenType::String,

        // Skip the first and last quotes
        .literal = _parent.data().substr(
          offset + 1,
          size - 1
        )
      };
    }

    Token int_consumer__::consume() {
      size_t offset = _parent.index();
      size_t size = 0;

      do {
        char c = _parent.current();

        if (!is_integer(c)) {
          break;
        }

        size++;
      } while (_parent.next());

      return Token{
        .ty = TokenType::Integer,
        .literal = _parent.data().substr(
          offset,
          size
        )
      };
    }

    bool is_asterisk(char c) {
      return c == '*';
    }

    bool is_quote(char c) {
      return c == '"';
    }

    bool is_ident(char c) {
      if (c >= 'a' && c <= 'z') {
        return true;
      }

      if (c >= 'A' && c <= 'Z') {
        return true;
      }

      if (c == '_') {
        return true;
      }

      return false;
    }

    bool is_equals(char c) {
      return c == '=';
    }

    bool is_comma(char c) {
      return c == ',';
    }
    
    bool is_integer(char c) {
      return c >= '0' && c <= '9';
    }

    bool is_space(char c) {
      return c == ' ';
    }

    bool is_open_bracket(char c) {
      return c == '(';
    }

    bool is_close_bracket(char c) {
      return c == ')';
    }

    bool is_semicolon(char c) {
      return c == ';';
    }
  }
}