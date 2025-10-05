#ifndef _SQL_PARSER_INTERNAL_READER_HH
#define _SQL_PARSER_INTERNAL_READER_HH

#include <vector>

#include "sql/lexer/lexer.hh"

namespace sql::parser::detail {
  class token_reader__ {
    private:
      const std::vector<sql::lexer::Token>& _tokens;
      size_t _index;

    public:
      token_reader__(
        const std::vector<sql::lexer::Token>& tokens
      ) : _tokens(tokens), _index(0) {}

      bool empty() const;

      const sql::lexer::Token& token() const;
      void next();
      void next_non_space();
      
      void back();

      void must(sql::lexer::Type ty);
      void must_one_of(
        std::initializer_list<sql::lexer::Type> tys
      );
      
      bool is(sql::lexer::Type ty);
      bool is_one_of(
        std::initializer_list<sql::lexer::Type> tys
      );
    };    
    
    void trim_left(token_reader__& reader);

}

#endif /* _SQL_PARSER_INTERNAL_READER_HH */