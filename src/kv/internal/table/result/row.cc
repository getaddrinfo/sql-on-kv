#include "row.hh"

namespace kv::detail::table::result {
  const value& Row::at(size_t index) const {
    return _values.at(index);
  }

  const size_t Row::size() const {
    return _values.size();
  }
}