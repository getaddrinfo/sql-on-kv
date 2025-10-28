#include "statement.hh"

namespace sql::parser::statement {
  const std::string& operator_name(const Operator op) {
    static std::unordered_map<Operator, const std::string> lookup = {
      {Operator::Equals, "Equals"},
      {Operator::Ne, "NotEquals"},
      {Operator::And, "And"},
      {Operator::Or, "Or"}
    };

    CHECK(lookup.contains(op)) << "Missing value in lookup: "
      << static_cast<int>(op);

    return lookup.at(op);
  }

  const size_t operator_precedence(const Operator op) {
    static std::unordered_map<Operator, size_t> lookup = {
      {Operator::Equals, 3},
      {Operator::Ne, 3},
      {Operator::And, 2},
      {Operator::Or, 1}
    };

    if (lookup.contains(op)) {
      return lookup.at(op);
    }

    return 0;
  }

  std::shared_ptr<where> condition::lhs() const {
    return _lhs;
  }
  
  std::shared_ptr<where> condition::rhs() const {
    return _rhs;
  }

  const Operator condition::op() const {
    return _operator;
  }

  const std::string& comparison::column() const {
    return _column;
  }

  const std::string& comparison::value() const {
    return _value;
  }

  const Operator comparison::op() const {
    return _operator;
  }
}