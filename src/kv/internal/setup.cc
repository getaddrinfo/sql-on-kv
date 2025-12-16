#include "setup.hh"
#include "network_thread.hh"

#include "absl/log/check.h"

#include <foundationdb/fdb_c.h>

namespace kv::detail {
  void setup() {
    ::fdb_select_api_version(720);
  }
}