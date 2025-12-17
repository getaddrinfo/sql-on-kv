#include "row.hh"

namespace kv::detail::table::result {
  const value& row::at(size_t index) const {
    return _values.at(index);
  }

  const size_t row::size() const {
    return _values.size();
  }
}