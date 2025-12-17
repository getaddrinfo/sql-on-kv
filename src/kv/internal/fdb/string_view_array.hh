#ifndef _KV_INTERNAL_FDB_STRING_VIEW_ARRAY_HH
#define _KV_INTERNAL_FDB_STRING_VIEW_ARRAY_HH

#include "future.hh"

#include <string_view>
#include <vector>

namespace kv::detail::fdb {
  /**
   * A wrapper over a raw string array returned by `fdb_c`.
   * 
   * Maintains a reference to `Future`, keeping the underlying
   * memory alive as long as this class is alive.
   */
  class string_view_array {
  private:
    /**
     * The future that this array is from.
     */
    future _owner;

    /**
     * Views to the `char*` values returned by `fdb_c`.
     */
    std::vector<std::string_view> _views;

  public:
    /**
     * Constructs a `string_view_array` from a future.
     * @note Fails if the future does not hold a string array.
     */
    string_view_array(future source);

    /**
     * Gets the underlying vector that holds the string views.
     */
    const std::vector<std::string_view>& get() const;

    /**
     * The number of string views held.
     */
    size_t size() const noexcept;

    /**
     * Gets the string view at the given index.
     * 
     * @note Throws if out of bounds.
     */
    std::string_view at(size_t index) const;
  };
}

#endif /* _KV_INTERNAL_FDB_STRING_VIEW_ARRAY_HH */