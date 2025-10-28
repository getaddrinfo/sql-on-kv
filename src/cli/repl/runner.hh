#ifndef _CLI_REPL_RUNNER_HH
#define _CLI_REPL_RUNNER_HH

#include <stdlib.h>

#include <memory>
#include <vector>

#include "result.hh"

namespace repl {
  class runner {
  private:
    size_t _count;
    std::vector<std::shared_ptr<Result>> _results;

  public:
    // TODO: Add a database connection as a parameter to this.
    runner() : _count(0) {}

    std::shared_ptr<const Result> eval(const std::string input);
  };
}

#endif /* _CLI_REPL_RUNNER_HH */