#include "future.hh"
#include "key_value_array.hh"
#include "string_view_array.hh"

#include <foundationdb/fdb_c.h>

#include "absl/log/check.h"

namespace kv::detail::fdb {
  void future_deleter::operator()(FDBFuture* ptr) {
    ::fdb_future_destroy(ptr);
  }

  void Future::block_until_ready() {
    CHECK_EQ(::fdb_future_block_until_ready(raw()), 0)
      << "future failed to block until ready";
  }

  bool Future::ready() const {
    fdb_bool_t is_ready = ::fdb_future_is_ready(raw());
    return is_ready != 0;
  }

  template <>
  int64_t Future::value<int64_t>() {
    int64_t out;
    fdb_error_t err = ::fdb_future_get_int64(raw(), &out);

    CHECK_EQ(err, 0) << "::fdb_future_get_double returned non-zero error";

    return out;
  }

  template <>
  uint64_t Future::value<uint64_t>() {
    uint64_t out;
    fdb_error_t err = ::fdb_future_get_uint64(raw(), &out);

    CHECK_EQ(err, 0) << "::fdb_future_get_uint64 returned non-zero error";

    return out;
  }

  template <>
  bool Future::value<bool>() {
    fdb_bool_t out;
    fdb_error_t err = ::fdb_future_get_bool(raw(), &out);

    CHECK_EQ(err, 0) << "::fdb_future_get_bool returned non-zero error";

    return out != 0;
  }

  template <>
  double Future::value<double>() {
    double out;
    fdb_error_t err = ::fdb_future_get_double(raw(), &out);

    CHECK_EQ(err, 0) << "::fdb_future_get_double returned non-zero error";

    return out;
  }

  template <>
  string_view_array Future::value<string_view_array>() {
    string_view_array out(*this);
    return out;
  }


  template <>
  key_value_array Future::value<key_value_array>() {
    key_value_array out(*this);
    return out;
  }
}