#ifndef _SQL_PARSER_INTERNAL_WHERE_SHUNT_HH
#define _SQL_PARSER_INTERNAL_WHERE_SHUNT_HH

#include <memory>

#include "sql/parser/statement.hh"

namespace detail {

  using sql::parser::statement::where;
  using sql::parser::statement::Operator;

  std::shared_ptr<where> shunt(
    std::vector<std::shared_ptr<where>>& conditions,
    std::vector<Operator>& operators 
  );
}

#endif /* _SQL_PARSER_INTERNAL_WHERE_SHUNT_HH */