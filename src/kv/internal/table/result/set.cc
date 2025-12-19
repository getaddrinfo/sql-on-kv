#include "set.hh"

namespace kv::detail::table::result {
  void Set::add(std::vector<Value> values) {
    add(Row(values));
  }

  void Set::add(Row r) {
    _rows.push_back(r);
  }
}