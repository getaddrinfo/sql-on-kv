#ifndef _KV_INTERNAL_TABLE_RESULT_COLUMN_HH
#define _KV_INTERNAL_TABLE_RESULT_COLUMN_HH

#include <vector>

#include "row.hh"

namespace kv::detail::table::result {
  class set {
  private:
    std::vector<row> _rows;
    size_t _dimension;
  public: 
    set(size_t dimension) : _dimension(dimension) {}

    void add(row);
    void add(std::vector<value> values);
  };
}

#endif /* _KV_INTERNAL_TABLE_RESULT_COLUMN_HH */