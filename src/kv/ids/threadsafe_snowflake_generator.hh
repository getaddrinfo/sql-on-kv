#ifndef _KV_IDS_THREADSAFE_SNOWFLAKE_GENERATOR_HH
#define _KV_IDS_THREADSAFE_SNOWFLAKE_GENERATOR_HH

#include <inttypes.h>
#include "snowflake_generator.hh"

#include <mutex>

class threadsafe_snowflake_generator : snowflake_generator {
private:
  std::mutex _mutex;

public:
  threadsafe_snowflake_generator(
    uint64_t epoch,
    uint8_t node_id,
    uint8_t datacenter_id
  ) : snowflake_generator(epoch, node_id, datacenter_id) {}

  uint64_t next() override;
};

#endif /* _KV_IDS_THREADSAFE_SNOWFLAKE_GENERATOR_HH */