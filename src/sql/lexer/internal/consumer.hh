#ifndef _SQL_LEXER_INTERNAL_CONSUMER_HH
#define _SQL_LEXER_INTERNAL_CONSUMER_HH

#include "state.hh"

namespace sql::lexer::detail {
  class ident_consumer__ {
  private:
    consumption_state__& _parent;
  
  public:
    ident_consumer__(
      consumption_state__& parent
    ) : _parent(parent) {}

    Token consume();
  };

  class string_consumer__ {
  private:
    consumption_state__& _parent;
  public:
    string_consumer__(
      consumption_state__& parent
    ) : _parent(parent) {}

    Token consume();
  };

  class int_consumer__ {
  private:
    consumption_state__& _parent;
  public:
    int_consumer__(
      consumption_state__& parent
    ) : _parent(parent) {}

    Token consume();
  };
}

#endif /* _SQL_LEXER_INTERNAL_CONSUMER_HH */