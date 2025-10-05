#include "consumer.hh"
#include "charset.hh"

#include <unordered_map>

namespace sql::lexer::detail {

  static std::unordered_map<std::string_view, Type> _keywords = {
    {"AND", Type::And},
    {"OR", Type::Or},
    {"IN", Type::In},
    {"INSERT", Type::Insert},
    {"INTO", Type::Into},
    {"SELECT", Type::Select},
    {"DELETE", Type::Delete},
    {"FROM", Type::From},
    {"WHERE", Type::Where},
    {"LIMIT", Type::Limit},
    {"VALUES", Type::Values},
    {"UPDATE", Type::Update},
    {"SET", Type::Set},
    {"CREATE", Type::Create},
    {"DROP", Type::Drop},
    {"TABLE", Type::Table}
  };

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

        return _parent.yield(
          _keywords.find(current_word)->second,
          offset,
          size
        );
      }
    } while (_parent.next());

    return _parent.yield(Type::Ident, offset, size);
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

    return _parent.yield(
      Type::String,
      // Skip the first and last quotes
      offset + 1,
      size - 1
    );
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

    return _parent.yield(
      Type::Integer,
      offset,
      size
    );
  }
}