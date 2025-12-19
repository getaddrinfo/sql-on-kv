#include "statement_parser.hh"
#include "reader.hh"
#include "statement.hh"

#include "absl/log/check.h"
#include "absl/log/log.h"

#include "where/parser.hh"

#include <unordered_set>
#include <unordered_map>
#include <functional>

namespace sql::parser::detail::statement::parser {
  using lexer::Type;

  sql::parser::statement::Select parse_select(detail::TokenReader& reader) {
    std::vector<std::string> fields;

    reader.must(Type::Select);
    reader.next();
    
    detail::trim_left(reader);

    do {
      // We must have an ident (ignoring the fact that
      // functions can be used in these places)
      reader.must(Type::Ident);

      // Consume the token, and add the fields to the strings
      const sql::lexer::Token& str = reader.token();
      fields.push_back(std::string(str.literal()));
      
      reader.next();
      
      // Skip a comma so that we can check if there is another ident
      // token to treat as a field.
      if (reader.is(Type::Comma)) {
        reader.next();
      }

      detail::trim_left(reader);
    } while (!reader.is_one_of({Type::From, Type::End}));

    // TODO: Move to VLOG
    for(const std::string& field : fields) {
      LOG(INFO) << "- " << field;
    }

    // Next part must be a `FROM` statement.
    reader.must(Type::From);
    reader.next();
    detail::trim_left(reader);

    // Next part must be a table name.
    reader.must(Type::Ident);
    std::string table(reader.token().literal());

    reader.next();
    detail::trim_left(reader);

    LOG(INFO) << "Table: " << table;

    // TODO: After this, most statements can vary in order, so we should instead use a
    // dispatcher map that parses on the token type.
    ::sql::parser::statement::detail::where::parse(reader);

    // TODO: Build the statement from the partial state produced by the parsers.
    return sql::parser::statement::Select{};
  }
}