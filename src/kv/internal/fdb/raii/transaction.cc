#include "transaction.hh"

namespace kv::detail::fdb {
  future transaction::get(const std::span<uint8_t> key, bool in_snapshot) {
    FDBFuture* fut = ::fdb_transaction_get(
      _ptr.get(),
      key.data(),
      key.size()
    );
      
    return future(fut);
  }
}