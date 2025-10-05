#include <string>

#include "sql/lexer/lexer.hh"
#include "sql/parser/parser.hh"

#include "absl/log/log.h"

int main(void) {
  const std::string code = "SELECT id, name FROM users WHERE ((id = 1 AND id=2) OR (name = \"Test\"));";

  std::vector<sql::lexer::Token> tokens = sql::lexer::lex(code);
  sql::parser::ParseResult result = sql::parser::parse(tokens);
  (void)result;
}