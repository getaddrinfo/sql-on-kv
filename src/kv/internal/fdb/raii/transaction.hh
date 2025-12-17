#ifndef _KV_INTERNAL_FDB_TRANSACTION_HH
#define _KV_INTERNAL_FDB_TRANSACTION_HH

#include <foundationdb/fdb_c.h>

#include <memory>

#include "absl/log/check.h"

#include "future.hh"

namespace kv::detail::fdb {
  struct transaction_deleter {
    void operator()(FDBTransaction* ptr);
  };

  /**
   * Represents a transaction on the given database.
   */
  class transaction {
  private:
    std::shared_ptr<FDBTransaction> _ptr;

  public:
    /**
     * Creates a transaction.
     */
    transaction(std::weak_ptr<FDBDatabase> db) {
      FDBTransaction* ptr = nullptr;
      fdb_error_t err = ::fdb_database_create_transaction(
        db.lock().get(),
        &ptr
      );

      CHECK_EQ(err, 0) << "::fdb_database_create_transaction failed with non-zero error code";
      CHECK_NE(ptr, nullptr) << "::fdb_database_create_transaction succeeded but ptr is nullptr";

      _ptr = std::shared_ptr<FDBTransaction>(ptr, transaction_deleter{});
    }

    /**
     * Gets a given key from the database.
     * @param key The key to get.
     * @param in_snapshot True if this is a snapshot read.
     */
    future get(const std::span<uint8_t> key, bool in_snapshot);
  };
}

#endif /* _KV_INTERNAL_FDB_TRANSACTION_HH */