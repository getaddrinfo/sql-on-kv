#ifndef _KV_INTERNAL_TABLE_RESULT_VALUE_HH
#define _KV_INTERNAL_TABLE_RESULT_VALUE_HH

#include <variant>
#include <string>

#include <inttypes.h>

#include "absl/log/check.h"

namespace kv::detail::table::result {
  using SqlValue = std::variant<
    int8_t,
    int16_t,
    int32_t,
    int64_t,
    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t,
    std::string
  >;

  class value {
    SqlValue _value;

    value(int8_t v)      : _value(v) {}
    value(int16_t v)     : _value(v) {}
    value(int32_t v)     : _value(v) {}
    value(int64_t v)     : _value(v) {}
    value(uint8_t v)     : _value(v) {}
    value(uint16_t v)    : _value(v) {}
    value(uint32_t v)    : _value(v) {}
    value(uint64_t v)    : _value(v) {}
    value(std::string v) : _value(v) {}

    /**
     * Returns true if the `value` holds type `T`.
     */
    template <typename T>
    bool is() { 
      return std::holds_alternative<T>(_value)
    }

    /**
     * Gets a reference to the value being held.
     * 
     * @note Terminates if there is an invalid access.
     */
    template <typename T>
    T& get() {
      CHECK(is<T>()) << "Must be of type T";

      return std::get<T&>(_value);
    }
  };
}

#endif /* _KV_INTERNAL_TABLE_RESULT_VALUE_HH */