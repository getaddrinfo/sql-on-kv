#include "state.hh"
#include "charset.hh"
#include "consumer.hh"

#include <unordered_map>

#include "absl/log/check.h"
#include "absl/strings/str_format.h"

namespace sql::lexer::detail {
  static std::unordered_map<char, Type> _yieldables = {
    {',', Type::Comma},
    {';', Type::Semicolon},
    {'*', Type::Asterisk},
    {' ', Type::Space},
    {'(', Type::RightParen},
    {')', Type::LeftParen},
    {'=', Type::Equals}
  };

  Token ConsumptionState::yield(Type type) {
    Token token = yield(
      type,
      index(),
      1
    );

    next();
    return token;
  }

  Token ConsumptionState::yield(
    Type type,
    size_t offset,
    size_t length
  ) {
    Token token(type, _data.substr(offset, length));    
    return token;
  }

  Token ConsumptionState::consume() {
    char curr = current();

    if (_yieldables.contains(curr)) {
      return yield(_yieldables.at(curr));
    }

    if (is_quote(curr)) {
      detail::StringConsumer consumer(*this);
      return consumer.consume();
    }

    if (is_integer(curr)) {
      detail::IntConsumer consumer(*this);
      return consumer.consume();
    }

    if (is_ident(curr)) {
      detail::IdentConsumer consumer(*this);
      return consumer.consume();
    }

    CHECK(false) << absl::StrFormat(
      "unreachable: input \"%c\" unmatched",
      curr
    );
  }

  bool ConsumptionState::next() {
    if (_curr_index + 1 == _data.size()) {
      return false;
    }
    
    _curr_index++;
    return true;
  }

  void ConsumptionState::back() {
    if (_curr_index == 0) {
      return;
    }

    _curr_index--;
  }

  const char ConsumptionState::current() const {
    CHECK(_curr_index < _data.size()) << "Out of bounds read: idx " 
      << _curr_index
      << " of "
      << _data.size()
      << " elements.";

    return _data.at(_curr_index);
  }

  const size_t ConsumptionState::index() const {
    return _curr_index;
  }
  
  const bool ConsumptionState::finished() const {
    return _curr_index + 1 == _data.size();
  }

  const std::string_view ConsumptionState::data() const {
    return _data;
  }
}