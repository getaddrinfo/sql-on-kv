#include "connection.hh"

#include "absl/log/check.h"


namespace kv {
  // Destroy the database using `fdb_database_destroy`.
  void Connection::database_deleter::operator()(FDBDatabase* ptr) {
    ::fdb_database_destroy(ptr);
  }

  std::weak_ptr<FDBDatabase> Connection::get() {
    if (_db.get() == nullptr) {
      initialize();
    }

    return std::weak_ptr<FDBDatabase>(_db);
  }

  void Connection::initialize() {
    FDBDatabase* ptr;
    fdb_error_t err = ::fdb_create_database(_cluster_path.c_str(), &ptr);

    CHECK_EQ(err, 0) << "fdb error occurred";
    CHECK_NE(ptr, NULL) << "::fdb_create_database did not error, but db is nullptr.";

    _db = std::shared_ptr<FDBDatabase>(ptr, database_deleter{});
  }
}