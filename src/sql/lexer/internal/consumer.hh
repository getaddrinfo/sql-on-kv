#ifndef _SQL_LEXER_INTERNAL_CONSUMER_HH
#define _SQL_LEXER_INTERNAL_CONSUMER_HH

#include "state.hh"

namespace sql::lexer::detail {
  /**
   * Consumes an identity from the consumption state.
   */
  class ident_consumer__ {
  private:
    /**
     * The consumption state this consumer is consuming from.
     */
    consumption_state__& _parent;
  
  public:
    ident_consumer__(
      consumption_state__& parent
    ) : _parent(parent) {}

    /**
     * Returns a token that represents an identity.
     * 
     * @throws
     */
    Token consume();
  };

  /**
   * Consumes a string from the consumption state.
   */
  class string_consumer__ {
  private:
    /**
     * The consumption state this consumer is consuming from.
     */
    consumption_state__& _parent;
  public:
    string_consumer__(
      consumption_state__& parent
    ) : _parent(parent) {}

    /**
     * Returns a token that represents a string
     * 
     * @throws
     */
    Token consume();
  };

  /**
   * Consumes an integer from the consumption state.
   */
  class int_consumer__ {
  private:
    /**
     * The consumption state this consumer is consuming from.
     */
    consumption_state__& _parent;
  public:
    int_consumer__(
      consumption_state__& parent
    ) : _parent(parent) {}

    /**
     * Returns a token that represents an integer.
     * 
     * @throws
     */
    Token consume();
  };
}

#endif /* _SQL_LEXER_INTERNAL_CONSUMER_HH */