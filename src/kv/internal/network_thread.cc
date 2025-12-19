#include <foundationdb/fdb_c.h>

#include "absl/log/check.h"

#include <memory>

#include "network_thread.hh"

namespace kv::detail::fdb {
  void run() {
    ::fdb_run_network();
  }

  void NetworkThread::stop() {
    // This must finish blocking before `thread->join` is called
    // as per foundationdb docs.
    ::fdb_stop_network();

    _thread->join();
  }

  NetworkThread::NetworkThread() : _thread(std::make_unique<std::thread>(&run)) {}

  NetworkThread::~NetworkThread() {
    stop();
  }
}