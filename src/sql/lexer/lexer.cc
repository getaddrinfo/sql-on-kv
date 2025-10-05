#include "lexer.hh"

#include <unordered_map>

#include "absl/log/log.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"

#include <string>
#include <string_view>

#include "internal/state.hh"

namespace sql::lexer {
  std::vector<Token> lex(const std::string& source) {
    std::vector<Token> tokens;
    std::string_view view(source);
    detail::consumption_state__ tokeniser(view);

    while (!tokeniser.finished()) {
      sql::lexer::Token token = tokeniser.consume();

      // TODO: VLOG
      LOG(INFO) 
        << token.type_name()
        << '('
        << token.literal()
        << ')';

      tokens.push_back(token);
    }

    tokens.emplace_back(sql::lexer::Type::End, tokeniser.data().substr(0, tokeniser.index() - 1));

    return tokens;
  }
}