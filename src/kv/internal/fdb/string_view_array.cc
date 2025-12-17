#include "string_view_array.hh"

#include "absl/log/check.h"

namespace kv::detail::fdb {
  string_view_array::string_view_array(future source) : _owner(source) {
    char** ptr;
    int size;

    fdb_error_t err = ::fdb_future_get_string_array(
      source.raw(),
      &ptr,
      &size
    );

    CHECK_EQ(err, 0) << "::fdb_future_get_string_array returned non-zero error code";

    _views.reserve(size);

    for(int i = 0; i < size; i++) {
      _views[i] = std::string_view(ptr[i]);
    }
  }

  const std::vector<std::string_view>& string_view_array::get() const noexcept {
    return _views;
  }

  std::string_view string_view_array::at(size_t index) const {
    return _views.at(index);
  }

  size_t string_view_array::size() const noexcept {
    return _views.size();
  }
}