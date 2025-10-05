#include <string>

#include "sql/lexer.hh"
#include "sql/parser.hh"

#include "absl/log/log.h"

int main(void) {
  const std::string code = "SELECT id, name FROM users WHERE ((id = 1 AND id=2) OR (id=3));";

  std::vector<sql::lexer::Token> tokens = sql::lexer::lex(code);
  sql::parser::ParseResult result = sql::parser::parse(tokens);
  (void)result;
}