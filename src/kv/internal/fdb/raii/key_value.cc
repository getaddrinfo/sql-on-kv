#include "key_value.hh"

namespace kv::detail::fdb {
  const std::span<uint8_t> KeyValue::key() const {
    return _key;
  }

  const std::span<uint8_t> KeyValue::value() const {
    return _value;
  }
}