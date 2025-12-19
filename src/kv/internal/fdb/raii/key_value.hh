#ifndef _KV_INTERNAL_FDB_KEY_VALUE_HH
#define _KV_INTERNAL_FDB_KEY_VALUE_HH

#include <foundationdb/fdb_c.h>

#include <span>

#include "future.hh"

namespace kv::detail::fdb {
  /**
   * Wraps a key-value pair into a class holding spans over
   * the memory. Produced as part of a `key_value_array` which 
   * maintains ownership over the values.
   */
  class KeyValue {
  private:
    /**
     * A view into the bytes of the key.
     */
    std::span<uint8_t> _key;

    /**
     * A view into the bytes of the value.
     */
    std::span<uint8_t> _value;

  public:
    /**
     * Constructs a `key_value` from the raw `FDBKeyValue` from
     * `fdb_c`. This class does not maintain ownership of a reference
     * to `Future`, so it is not guaranteed to exist beyond the scope
     * of the `key_value_array` which produced it.
     */
    KeyValue(FDBKeyValue kv) {
      _key = std::span(
        (uint8_t*)kv.key,
        kv.key_length
      );

      _value = std::span(
        (uint8_t*)kv.value,
        kv.value_length
      );
    }

    /**
     * Get the key of this key-value pair.
     */
    const std::span<uint8_t> key() const;

    /**
     * Get the value of this key-value pair.
     */
    const std::span<uint8_t> value() const;
  };
}

#endif /* _KV_INTERNAL_FDB_KEY_VALUE_HH */