#include "threadsafe_snowflake_generator.hh"

uint64_t threadsafe_snowflake_generator::next() {
  std::lock_guard lock_(_mutex);
  return snowflake_generator::next();
}