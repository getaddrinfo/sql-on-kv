#ifndef _KV_INTERNAL_FDB_FUTURE_HH
#define _KV_INTERNAL_FDB_FUTURE_HH

#include <foundationdb/fdb_c.h>
#include <memory>

namespace kv::detail::fdb {
  /**
   * A deleter for futures, calling `::fdb_future_destroy` once
   * the refcount has reached `0`.
   */
  struct future_deleter {
    void operator()(FDBFuture* ptr);
  };

  using FuturePtr = std::shared_ptr<FDBFuture>;

  /**
   * A wrapper over `FDBFuture*`, managing memory ownership.
   */
  class future {
  private:
    /**
     * The `FDBFuture` as a `shared_ptr` with a custom deleter.
     */
    FuturePtr _ptr;

  public:
    future(FDBFuture* ptr) : _ptr(ptr, future_deleter{}) {}

    /**
     * Gets the raw pointer for operations via the `fdb_c`
     * api. Consumers passing this into another class should
     * instead use `get()` as this tracks references.
     */
    FDBFuture* raw() const;

    /**
     * Gets a pointer to the `FDBFuture`.
     */
    FuturePtr get() const;

    /**
     * Returns true if the future is ready.
     */
    bool ready() const;

    /**
     * Blocks until the future is ready
     * @note Fails if the underlying wait mechanism fails.
     */
    void block_until_ready();

    /**
     * Read a `T` from the future.
     * @note Fails if the future doesn't hold a `T`.
     */
    template <typename T>
    T value();
  };
}

#endif /* _KV_INTERNAL_FDB_FUTURE_HH */