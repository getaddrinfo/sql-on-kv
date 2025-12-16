#ifndef _KV_SNOWFLAKE_GENERATOR_HH
#define _KV_SNOWFLAKE_GENERATOR_HH

#include <inttypes.h>

class snowflake_generator {
private:
  const int64_t _epoch;
  const uint8_t _node_id;
  const uint8_t _datacenter_id;


  uint16_t _sequence;
  int64_t _last;

public:
  snowflake_generator(
    int64_t epoch,
    uint8_t node_id,
    uint8_t datacenter_id
  ) : _epoch(epoch), _node_id(node_id), _datacenter_id(datacenter_id) {}

  snowflake_generator(snowflake_generator&) = delete;
  snowflake_generator& operator=(snowflake_generator&) = delete;

  virtual uint64_t next();
};

#endif /* _KV_SNOWFLAKE_GENERATOR_HH */