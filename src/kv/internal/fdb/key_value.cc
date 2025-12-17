#include "key_value.hh"

namespace kv::detail::fdb {
  const std::span<uint8_t> key_value::key() const {
    return _key;
  }

  const std::span<uint8_t> key_value::value() const {
    return _value;
  }
}