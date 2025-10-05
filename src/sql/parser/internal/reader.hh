#ifndef _SQL_PARSER_INTERNAL_READER_HH
#define _SQL_PARSER_INTERNAL_READER_HH

#include <vector>

#include "sql/lexer/lexer.hh"

namespace sql::parser::detail {
  /**
   * Represents a reader of `Token`s that tracks current position
   * and safeguards against out of bounds reads.
   */
  class token_reader__ {
    private:
      /**
       * A const reference to the tokens produced by lexing.
       */
      const std::vector<sql::lexer::Token>& _tokens;

      /**
       * The current index that the reader is at.
       */
      size_t _index;

    public:
      token_reader__(
        const std::vector<sql::lexer::Token>& tokens
      ) : _tokens(tokens), _index(0) {}

      /**
       * Returns true if the token reader is empty.
       */
      bool empty() const;

      /**
       * Returns the current token.
       */
      const sql::lexer::Token& token() const;

      /**
       * Moves to the next token.
       */
      void next();

      /**
       * Moves to the next non-space token.
       */
      void next_non_space();

      /**
       * Moves to the previous token.
       */
      void back();

      /**
       * Calls `CHECK` on the result of `is(ty)`. If the current token (from `token()`)
       * is not of type `ty`, then an assert will fail (via `abseil`'s `CHECK`).
       * 
       * Results in a `noreturn` on mismatch.
       * 
       * @returns void
       */
      void must(sql::lexer::Type ty);
      
      /**
       * Ensures that the current token (via `token()`)'s type is in the list of 
       * provided types. If not, an assertion fails after no match occurs.
       * 
       * Results in a noreturn on no match.
       *
       * @returns void 
       */
      void must_one_of(
        std::initializer_list<sql::lexer::Type> tys
      );


      /**
       * Returns true if the current token is of type `ty`.
       * 
       * @returns bool
       */
      bool is(sql::lexer::Type ty);

      /**
       * Returns true if the current token is of one of the types `tys`.
       * 
       * @returns bool
       */
      bool is_one_of(
        std::initializer_list<sql::lexer::Type> tys
      );
    };    
    
    /**
     * Consumes spaces until the reader is at a non-space
     * token (which may be the end of the tokens).
     */
    void trim_left(token_reader__& reader);

}

#endif /* _SQL_PARSER_INTERNAL_READER_HH */