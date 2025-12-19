#ifndef _KV_INTERNAL_FDB_NETWORK_THREAD_HH
#define _KV_INTERNAL_FDB_NETWORK_THREAD_HH

#include <memory>
#include <thread>
#include <atomic>

namespace kv::detail::fdb {
  void run();

  // TODO: Copy + Move constructors.
  class NetworkThread {
    std::unique_ptr<std::thread> _thread;

  public:  
    NetworkThread();
    NetworkThread(NetworkThread&) = delete;
    NetworkThread& operator=(NetworkThread&) = delete;
    ~NetworkThread();
  
  private:
      void stop();
  };
}

#endif /* _KV_INTERNAL_FDB_NETWORK_THREAD_HH */
