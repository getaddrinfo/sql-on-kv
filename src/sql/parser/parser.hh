#ifndef _SQL_PARSER_PARSER_HH
#define _SQL_PARSER_PARSER_HH

#include "sql/lexer/lexer.hh"

#include "statement.hh"

#include <variant>

namespace sql::parser {
  // TODO: Each member of this should be a `std::unique_ptr`
  // since stack allocating these objects could get large.
  //
  // TODO: Should this really be done using polymorphism?
  // 
  // We still need to do the discrimination upon the base class
  // using either a `type()` method, or 
  // `dynamic_cast<Desired*>(ptr) != nullptr`.
  using ParseResult = std::variant<
    statement::select,
    statement::update,
    statement::insert,
    statement::delete_row,
    statement::create_table,
    statement::drop_table
  >;

  ParseResult parse(const std::vector<sql::lexer::Token>& tokens);
}

#endif /* _SQL_PARSER_HH */