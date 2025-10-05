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

    /**
     * Moves to the next character.
     * 
     * Returns true if the head was moved, false otherwise. Useful when
     * consuming in a loop (e.g., an int consumer).
     * 
     * @returns bool
     */
    bool next();

    /**
     * Moves to the previous character.
     */
    void back();

    /**
     * Returns the next complete token. 
     */
    Token consume();
    
    /**
     * Produces a token that represents the given type 
     * with a view to the given index and length.
     */
    Token yield(
      Type type,
      size_t index,
      size_t len
    );

    /**
     * Produces a token that represents the given type,
     * with a view to only 1 character at the current head.
     */
    Token yield(
      Type type
    );

    /**
     * The current character at the head of the consumer.
     */
    const char current() const;

    /**
     * The current head index.
     */
    const size_t index() const;

    /**
     * Gets a const reference to the data that this consumer is reading from.
     */
    const std::string_view data() const;

    /**
     * Returns true if the consumer is empty (fully consumed), false otherwise.
     */
    const bool finished() const;
  };
}

#endif /* _SQL_LEXER_INTERNAL_STATE_HH */