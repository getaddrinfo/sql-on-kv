#ifndef _SQL_LEXER_INTERNAL_CONSUMER_HH
#define _SQL_LEXER_INTERNAL_CONSUMER_HH

#include "state.hh"

namespace sql::lexer::detail {
  /**
   * Consumes an identity from the consumption state.
   */
  class IdentConsumer {
  private:
    /**
     * The consumption state this consumer is consuming from.
     */
    ConsumptionState& _parent;
  
  public:
    IdentConsumer(
      ConsumptionState& parent
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
  class StringConsumer {
  private:
    /**
     * The consumption state this consumer is consuming from.
     */
    ConsumptionState& _parent;
  public:
    StringConsumer(
      ConsumptionState& parent
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
  class IntConsumer {
  private:
    /**
     * The consumption state this consumer is consuming from.
     */
    ConsumptionState& _parent;
  public:
    IntConsumer(
      ConsumptionState& parent
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