#include <foundationdb/fdb_c.h>

#include "absl/log/check.h"

#include <memory>

namespace kv::detail::fdb {
  void run() {
    ::fdb_run_network();
  }

  void network_thread::stop() {
    // This must finish blocking before `thread->join` is called
    // as per foundationdb docs.
    ::fdb_stop_network();

    _thread->join();
  }

  network_thread::network_thread() : _thread(std::make_unique<std::thread>(&run)) {}

  network_thread::~network_thread() {
    stop();
  }
}