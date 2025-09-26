#include <string>

#include "sql/lexer.hh"
#include "absl/log/log.h"

int main(void) {
  const std::string code = "SELECT * FROM users WHERE id IN (1, 2, 3);";

  std::vector<sql::lexer::Token> tokens = sql::lexer::lex(code);

  LOG(INFO) << tokens.size();
}