#ifndef _KV_INTERNAL_TABLE_RESULT_COLUMN_HH
#define _KV_INTERNAL_TABLE_RESULT_COLUMN_HH

#include <vector>

#include "row.hh"

namespace kv::detail::table::result {
  class Set {
  private:
    std::vector<Row> _rows;
    size_t _dimension;
  public: 
    Set(size_t dimension) : _dimension(dimension) {}

    void add(Row);
    void add(std::vector<Value> values);
  };
}

#endif /* _KV_INTERNAL_TABLE_RESULT_COLUMN_HH */