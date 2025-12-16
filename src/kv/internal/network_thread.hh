#ifndef _KV_INTERNAL_FDB_NETWORK_THREAD_HH
#define _KV_INTERNAL_FDB_NETWORK_THREAD_HH

#include <memory>
#include <thread>
#include <atomic>

namespace kv::detail::fdb {
  void run();

  // TODO: Copy + Move constructors.
  class network_thread {
    std::unique_ptr<std::thread> _thread;

  public:  
    network_thread();
    network_thread(network_thread&) = delete;
    network_thread& operator=(network_thread&) = delete;
    ~network_thread();
  
  private:
      void stop();
  };
}

#endif /* _KV_INTERNAL_FDB_NETWORK_THREAD_HH */
