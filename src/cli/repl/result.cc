#include "result.hh"

namespace repl {
  const size_t Result::index() const {
    return _index;
  }
  
  const Data& Result::data() const {
    return _data;
  }

  const std::string pretty_print(const Result& result) {
    return std::get<Success>(result.data()).output;
  }

}