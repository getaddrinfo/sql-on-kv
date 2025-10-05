#ifndef _SQL_PARSER_PARSER_HH
#define _SQL_PARSER_PARSER_HH

#include "sql/lexer/lexer.hh"

#include "statement.hh"

#include <variant>

namespace sql::parser {
  /**
   * A variant of all the types of queries that can be parsed by
   * the parser.
   * 
   * TODO: Is this better as an inheritance heirarchy?
   */
  using ParseResult = std::variant<
    statement::select,
    statement::update,
    statement::insert,
    statement::delete_row,
    statement::create_table,
    statement::drop_table
  >;

  /**
   * Parses a vector of `Token`s and returns a ParseResult holding the
   * parsed query.
   * 
   * @throws
   */
  ParseResult parse(const std::vector<sql::lexer::Token>& tokens);
}

#endif /* _SQL_PARSER_HH */