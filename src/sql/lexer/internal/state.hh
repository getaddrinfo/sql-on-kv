#ifndef _SQL_LEXER_INTERNAL_STATE_HH
#define _SQL_LEXER_INTERNAL_STATE_HH

#include <string_view>

#include "sql/lexer/token.hh"

namespace sql::lexer::detail {
  class consumption_state__ {
  private:
    const std::string_view _data;
    size_t _curr_index;

  public:
    consumption_state__(const std::string_view data)
      : _data(data), _curr_index(0) {}

    bool next();
    void back();

    Token consume();
    
    Token yield(
      Type type,
      size_t index,
      size_t len
    );

    Token yield(
      Type type
    );

    const char current() const;
    const size_t index() const;
    const std::string_view data() const;
    const bool finished() const;
  };
}

#endif /* _SQL_LEXER_INTERNAL_STATE_HH */