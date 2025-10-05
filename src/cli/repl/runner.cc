#include "runner.hh"

namespace repl {
  std::shared_ptr<const Result> runner::eval(const std::string input) {
    std::shared_ptr<Result> result = std::make_shared<repl::Result>(
      _count++,
      Success{ .output = "OK" }
    );

    _results.push_back(result);

    return _results.back();
  }
}