#include "parser.hh"

#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "absl/log/log.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"

#include "internal/reader.hh"
#include "internal/state.hh"
#include "internal/statement_parser.hh"

namespace sql::parser {
  using ParserFn = std::function<
    ParseResult (detail::token_reader__&)
  >;

  static std::unordered_map<sql::lexer::Type, ParserFn> _dispatcher = {
    {sql::lexer::Type::Select, [](detail::token_reader__& reader) {
      return ParseResult(detail::statement::parser::parse_select(reader));
    }},
  };

  ParseResult parse(
    const std::vector<sql::lexer::Token>& tokens
  ) {
    detail::token_reader__ reader(tokens);
    detail::trim_left(reader);

    if (reader.empty()) {
      // TODO: this
    }

    const sql::lexer::Token& token = reader.token();

    bool found = false;
    for(const auto& dispatchable : _dispatcher) {
      found = found || reader.is(dispatchable.first);
    }

    QCHECK(found) << "Not a valid token to start: "
      << reader.token().type_name();

    reader.back();

    ParserFn& parser = _dispatcher.at(token.type());
    return parser(reader);
  }
}