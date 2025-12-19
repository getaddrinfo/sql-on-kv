#ifndef _KV_CONNECTION_HH
#define _KV_CONNECTION_HH

#include <memory>

#include <foundationdb/fdb_c.h>

namespace kv {

  class Connection {
    struct database_deleter {
      void operator()(FDBDatabase* ptr);
    };

    // TODO: Move, copy constructors
  private:
    const std::string& _cluster_path;
    std::shared_ptr<FDBDatabase> _db;

  public:
    Connection(const std::string& cluster_path) : _cluster_path(cluster_path), _db(nullptr) {}
    ~Connection();

    /**
     * Gets a weak pointer to the database. Calls to the C API should upgrade this.
     * 
     * @note You are expected to check if it is expired.
     * @todo Performance implications of the above weak_ptr -> shared_ptr upgrades.
     */
    std::weak_ptr<FDBDatabase> get();
  
  private:
    void initialize();
  };
}

#endif /* _KV_CONNECTION_HH */