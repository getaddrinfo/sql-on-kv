#ifndef _SQL_PARSER_TYPE_HH
#define _SQL_PARSER_TYPE_HH

#include <inttypes.h>

#include <variant>

namespace sql::parser {
  /**
   * Represents the values that can be configured when creating or altering 
   * a type in a database.
   */
  namespace ty {
    /**
     * SQL `TEXT` type.
     * 
     * A type well-suited for long blobs of text. Applies compression.
     */
    struct Text {
      /**
       * The maximum length this field can hold.
       */
      uint32_t max_length;
    };

    /**
     * SQL `CHAR` type.
     * 
     * Represents a fixed buffer of `char`s, padded out with 0s to 
     * reach `max_length`.
     */
    struct Char {
      /**
       * The number of characters the buffer holds.
       */
      uint32_t max_length;
    };

    /**
     * SQL `VARCHAR` type.
     * 
     * Represents a variable buffer (up to `max_length` size) of characters, 
     * where the length is encoded into the stored representation.
     */
    struct VarChar {
      /**
       * The maximum length this value can be.
       */
      uint32_t max_length;
    };

    /**
     * UInt8 (0 to 255)
     */
    struct Uint8 {};

    /**
     * UInt16 (0 to 16,535)
     */
    struct Uint16 {};

    /**
     * UInt32 (0 to 4,294,967,295)
     */
    struct Uint32 {};

    /**
     * UInt64 (0 to 18,446,744,073,709,551,615)
     */
    struct Uint64{};

    /**
     * Int8 (-128 to 127)
     */
    struct Int8 {};

    /**
     * Int16 (-32,768 to 32,767)
     */
    struct Int16 {};

    /**
     * Int32 (-2,147,483,648 to 2,147,483,647)
     */
    struct Int32 {};

    /**
     * Int64 (-9,223,372,036,854,775,808 to 9,223,372,036,854,775,807)
     */
    struct Int64 {};

    /**
     * Boolean (0 or 1)
     */
    struct Bool {};

    /**
     * Bit (0 or 1) - Useful for tighter packing than a boolean.
     */
    struct Bit {};
  };
  
  /**
   * A variant that can contain all the types the database can contain.
   * 
   * Use the `type` being held in the variant to determine the type in 
   * the db.
   */
  using SqlType = std::variant<
    ty::Text,
    ty::Char,
    ty::VarChar,
    ty::Uint8,
    ty::Uint16,
    ty::Uint32,
    ty::Uint64,
    ty::Int8,
    ty::Int16,
    ty::Int32,
    ty::Int64
  >;
}

#endif /* _SQL_PARSER_TYPE_HH */