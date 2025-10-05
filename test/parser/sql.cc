#include "gtest/gtest.h"

#include <vector>

#include "sql/lexer.hh"
#include "sql/parser.hh"


TEST(Parser, ShouldParseSimpleStatement) {
  const std::string statement = "SELECT id, name FROM users;";

  std::vector<sql::lexer::Token> tokens = sql::lexer::lex(statement);
  auto _dont_care = sql::parser::parse(tokens);
}

TEST(Parser, ShouldParseSimpleStatementLackingSemicolon) {
  const std::string statement = "SELECT id, name FROM users";

  std::vector<sql::lexer::Token> tokens = sql::lexer::lex(statement);
  auto _dont_care = sql::parser::parse(tokens);
}

TEST(Parser, ShouldParseSimpleStatementWithWhereClause) {
  const std::string statement = "SELECT id, name FROM users WHERE (id = 1);";

  std::vector<sql::lexer::Token> tokens = sql::lexer::lex(statement);
  auto _dont_care = sql::parser::parse(tokens);
}

TEST(Parser, ShouldParseSimpleStatementWithWhereClauseLackingSemicolon) {
  const std::string statement = "SELECT id, name FROM users WHERE (id = 1)";

  std::vector<sql::lexer::Token> tokens = sql::lexer::lex(statement);
  auto _dont_care = sql::parser::parse(tokens);
}

TEST(Parser, ShouldParseSimpleStatementWithConjunctionsInWhereClause) {
  const std::string statement = "SELECT id, name FROM users WHERE (id = 1 AND name = \"Test\");";
  
  std::vector<sql::lexer::Token> tokens = sql::lexer::lex(statement);
  auto _dont_care = sql::parser::parse(tokens);
}

TEST(Parser, ShouldParseSimpleStatementWithConjunctionsInWhereClauseLackingSemicolon) {
  const std::string statement = "SELECT id, name FROM users WHERE (id = 1 AND name = \"Test\")";
  
  std::vector<sql::lexer::Token> tokens = sql::lexer::lex(statement);
  auto _dont_care = sql::parser::parse(tokens);
}

TEST(Parser, ShouldParseStatementWithManyConjunctionsInWhereClause) {
  const std::string statement = "SELECT id, name FROM users WHERE ((id = 1 AND name = \"Test\") OR id=2);";

  std::vector<sql::lexer::Token> tokens = sql::lexer::lex(statement);
  auto _dont_care = sql::parser::parse(tokens);
}
