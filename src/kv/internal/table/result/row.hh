#ifndef _KV_INTERNAL_TABLE_RESULT_ROW_HH
#define _KV_INTERNAL_TABLE_RESULT_ROW_HH

#include "value.hh"

#include <vector>

namespace kv::detail::table::result {
  class row {
  private:
    std::vector<value> _values;

  public:
    row(
      const std::vector<value> values
    ) : _values(values) {}

    const value& at(size_t index) const;
    const size_t size() const;
  };
}

#endif /* _KV_INTERNAL_TABLE_RESULT_ROW_HH */