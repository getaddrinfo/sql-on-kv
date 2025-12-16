#include <chrono>
#include "snowflake_generator.hh"

static constexpr int64_t seq_bits = 12;
static constexpr int64_t worker_id_bits = 5;
static constexpr int64_t datacenter_id_bits = 5;
static constexpr int64_t node_id_shift = seq_bits;
static constexpr int64_t datacenter_id_shift = seq_bits + worker_id_bits;
static constexpr int64_t timestamp_shift = seq_bits + worker_id_bits + datacenter_id_bits;

uint64_t snowflake_generator::next() {
  std::chrono::milliseconds ms = std::chrono::duration_cast<
    std::chrono::milliseconds
  >(std::chrono::steady_clock::now().time_since_epoch());

  const uint64_t delta = (ms.count() - _epoch);
  
  if (_last != delta) {
    _sequence = 0;
    _last = delta;
  }

  const uint64_t id = ((delta - _epoch) << timestamp_shift)
    | (_datacenter_id << datacenter_id_shift)
    | (_node_id << node_id_shift)
    | _sequence;

  _sequence++;

  return id;
}