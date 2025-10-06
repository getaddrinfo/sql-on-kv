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
}