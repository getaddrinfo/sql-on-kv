#include "token.hh"

#include <unordered_map>

#include "absl/log/check.h"
#include "absl/strings/str_format.h"

namespace sql::lexer {
  const std::string& Token::type_name(const Type ty) {
    static std::unordered_map<Type, const std::string> map = {
      {Type::Error, "Error"},
      {Type::Asterisk, "Asterisk"},
      {Type::Comma, "Comma"},
      {Type::Space, "Space"},
      {Type::Ident, "Ident"},
      {Type::Equals, "Equals"},
      {Type::Assign, "Assign"},
      {Type::Delim, "Delim"},
      {Type::LeftParen, "LeftParen"},
      {Type::RightParen, "RightParen"},
      {Type::TemplatePlaceholder, "TemplatePlaceholder"},
      {Type::Integer, "Integer"},
      {Type::String, "String"},
      {Type::And, "And"},
      {Type::Or, "Or"},
      {Type::Insert, "Insert"},
      {Type::Into, "Into"},
      {Type::Select, "Select"},
      {Type::Delete, "Delete"},
      {Type::From, "From"},
      {Type::Where, "Where"},
      {Type::Limit, "Limit"},
      {Type::Values, "Values"},
      {Type::Update, "Update"},
      {Type::Set, "Set"},
      {Type::Create, "Create"},
      {Type::Drop, "Drop"},
      {Type::Table, "Table"},
      {Type::Semicolon, "Semicolon"},
      {Type::End, "End"},
      {Type::In, "In"}
    };

    auto result = map.find(ty);
    CHECK(result != map.end()) << "Unknown token type: " << static_cast<int>(ty);

    return result->second;
  }

  const std::string& Token::type_name() const {
    return Token::type_name(type());
  }

  const std::string Token::repr() const {
    return absl::StrFormat(
      "%s(%s)",
      type_name(),
      literal()
    );
  }

  const Type Token::type() const {
    return _type;
  }

  const std::string_view Token::literal() const {
    return _literal;
  }
}