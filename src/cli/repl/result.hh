#ifndef _CLI_REPL_RESULT_HH
#define _CLI_REPL_RESULT_HH

#include <variant>
#include <string>

namespace repl {
  struct Success {
    const std::string output;
  };

  struct Failure {
    uint32_t code;
  };

  using Data = std::variant<
    Success,
    Failure
  >;

  struct Result {
  private:
    size_t _index;
    Data _data;
  
  public:
    Result(
      size_t index,
      Data data
    ) : _index(index), _data(data) {}

    const size_t index() const;
    const Data& data() const;
  };

  const std::string pretty_print(const Result& result);
};

#endif /* _CLI_REPL_RESULT_HH */