#ifndef _SQL_PARSER_TYPE_HH
#define _SQL_PARSER_TYPE_HH

#include <inttypes.h>

#include <variant>

namespace sql::parser {
  namespace ty {
    struct Text {
      uint32_t max_length;
    };

    struct Char {
      uint32_t max_length;
    };

    struct VarChar {
      uint32_t max_length;
    };

    struct Uint8 {};
    struct Uint16 {};
    struct Uint32 {};
    struct Uint64{};
    struct Int8 {};
    struct Int16 {};
    struct Int32 {};
    struct Int64 {};
    struct Bool {};
    struct Bit {};
  };

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