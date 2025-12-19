#include "key_value_array.hh"
#include "future.hh"

#include "absl/log/check.h"

namespace kv::detail::fdb {
  KeyValueArray::KeyValueArray(Future source) : _owner(source) {
    FDBKeyValue* ptr;
    int size = 0;

    fdb_error_t err = fdb_future_get_keyvalue_array(
      source.raw(),
      &ptr,
      &size
    );

    CHECK_EQ(err, 0) << "::fdb_future_get_keyvalue_array returned non-zero error";

    _entries.reserve(size);

    for(int i = 0; i < size; i++) {
      _entries[i] = key_value(ptr[i]);
    }
  }

  const size_t KeyValueArray::size() const noexcept {
    return _entries.size();
  }

  const std::vector<key_value>& KeyValueArray::get() const {
    return _entries;
  }

  const key_value& KeyValueArray::at(size_t index) const {
    return _entries.at(index);
  }
}