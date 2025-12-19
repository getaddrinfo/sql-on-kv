#ifndef _KV_INTERNAL_SERIALIZABLE_HH
#define _KV_INTERNAL_SERIALIZABLE_HH

#include <inttypes.h>
#include <vector>

namespace kv::detail {
  
  /**
   * Represents a behaviour that allows a class to be serialized to a byte stream.
   */
  class Serializable {
    virtual const std::vector<uint8_t> serialize() const = 0;
  };

  // TODO: deser
}

#endif /* _KV_INTERNAL_SERIALIZABLE_HH */