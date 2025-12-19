#ifndef _KV_INTERNAL_TABLE_RESULT_ROW_HH
#define _KV_INTERNAL_TABLE_RESULT_ROW_HH

#include "value.hh"

#include <vector>

namespace kv::detail::table::result {
  class Row {
  private:
    std::vector<Value> _values;

  public:
    Row(
      const std::vector<Value> values
    ) : _values(values) {}

    const Value& at(size_t index) const;
    const size_t size() const;
  };
}

#endif /* _KV_INTERNAL_TABLE_RESULT_ROW_HH */