#include "set.hh"

namespace kv::detail::table::result {
  void set::add(std::vector<value> values) {
    add(row(values));
  }

  void set::add(row r) {
    _rows.push_back(r);
  }
}