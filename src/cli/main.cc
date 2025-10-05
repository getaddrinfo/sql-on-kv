#include <string>

#include "sql/lexer/lexer.hh"
#include "sql/parser/parser.hh"

#include "absl/log/log.h"

#include "cli/repl/runner.hh"

#include <iostream>
#include <string>
#include <memory>

int main(void) {
  // const std::string code = "SELECT id, name FROM users WHERE ((id = 1 AND id=2) OR (name = \"Test\"));";

  // std::vector<sql::lexer::Token> tokens = sql::lexer::lex(code);
  // sql::parser::ParseResult result = sql::parser::parse(tokens);
  // (void)result;

  repl::runner runner;

  char c;
  bool exit = false;

  do {
    std::cout << ">> " << std::flush;
    std::string input;
    std::getline(std::cin, input);

    std::shared_ptr<const repl::Result> result = runner.eval(input);

    std::cout << '$' << result->index();

    if (std::holds_alternative<repl::Success>(result->data())) {
      std::cout << ": ";
    } else {
      std::cout << "! ";
    }

    std::cout << repl::pretty_print(*result.get());
    std::cout << std::endl;

  } while (true);
}