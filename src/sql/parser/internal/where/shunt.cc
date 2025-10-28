#include "shunt.hh"

#include <stack>

#include "absl/log/log.h"

namespace detail {
  using sql::parser::statement::operator_precedence;

  template <typename T>
  T pop_back(std::stack<T>& stack) {
    T val = stack.top();
    (void)stack.pop();

    return std::move(val);
  }

  std::shared_ptr<where> shunt(
    std::vector<std::shared_ptr<where>>& conditions,
    std::vector<Operator>& operators 
  ) {
    CHECK_EQ(operators.size(), conditions.size() - 1)
      << "conditions and junctions mismatch";
    
    std::stack<std::shared_ptr<where>> nodes;
    std::stack<Operator> ops;

    nodes.push(std::move(conditions.at(0)));

    for(size_t i = 0; i < operators.size(); ++i) {
      VLOG(2) << "SQL statement (i=" << i << ")";
      
      sql::parser::statement::Operator op = operators.at(i);
      std::shared_ptr<sql::parser::statement::where> right = std::move(conditions.at(i + 1));
      
      while (
        !ops.empty() 
        && operator_precedence(ops.top()) >= operator_precedence(op)
      ) {
        VLOG(2) << "Consume Operand Stack " << i;

        sql::parser::statement::Operator topOp = pop_back(ops);

        std::shared_ptr<sql::parser::statement::where> right = pop_back(nodes);
        std::shared_ptr<sql::parser::statement::where> left = pop_back(nodes);
        std::shared_ptr<sql::parser::statement::where> node = std::make_shared<sql::parser::statement::condition>(
          left,
          topOp,
          right
        );

        nodes.push(node);
      }

      ops.push(op);
      nodes.push(right);
    }

    while (!ops.empty()) {
      sql::parser::statement::Operator topOp = pop_back(ops);
      std::shared_ptr<sql::parser::statement::where> rightNode = pop_back(nodes);
      std::shared_ptr<sql::parser::statement::where> leftNode = pop_back(nodes);
      
      std::shared_ptr<sql::parser::statement::where> node = std::make_shared<sql::parser::statement::condition>(
        leftNode,
        topOp,
        rightNode
      );

      nodes.push(node);
    }

    return std::move(nodes.top());
  }
}