#ifndef _KV_INTERNAL_FDB_KEY_VALUE_ARRAY_HH
#define _KV_INTERNAL_FDB_KEY_VALUE_ARRAY_HH

#include "key_value.hh"

#include <vector>

namespace kv::detail::fdb {
  /**
   * Wraps a `FDBKeyValue` array in a safe class to interact with
   * and maintain ownership of a reference to `Future` to ensure
   * the underlying memory is not freed.
   */
  class KeyValueArray { 
  private:
    Future _owner;
    std::vector<key_value> _entries;

  public:
    /**
     * Constructs a `key_value_array` from a future.
     * @note Fails if the `future` doesn't hold a `keyvalue`.
     */
    KeyValueArray(Future source); 

    /**
     * The number of entries in this `key_value_array`.
     */
    const size_t size() const noexcept;

    /**
     * Gets the underlying vector that holds the key_value pairs.
     */
    const std::vector<key_value>& get() const;

    /**
     * Gets a reference to the `key_value` at the given index.
     */
    const key_value& at(size_t index) const;
  };
}


#endif /* _KV_INTERNAL_FDB_KEY_VALUE_ARRAY_HH */