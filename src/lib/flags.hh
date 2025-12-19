#ifndef _LIB_FLAGS_HH
#define _LIB_FLAGS_HH

#include <type_traits>

namespace lib {
  template <
    typename T, 
    std::enable_if_t<
      std::conjunction_v<
        std::is_enum<T>,
        std::is_integral<std::underlying_type_t<T>>
      >
    > = 0
  >
  class Flags {
    using BitType = std::underlying_type_t<T>;
    BitType _value;

  public:
    Flags(BitType value) : _value(value) {}

    const bool has(T flag) const {
      BitType mask = static_cast<BitType>(flag);

      return (_value & mask) == mask;
    }

    void set(T flag) {
      BitType mask = static_cast<BitType>(flag);
      
      _value |= mask;
    }
    void unset(T flag) {
      BitType mask = static_cast<BitType>(flag);

      _value &= (~mask);
    }

    void toggle(T flag) {
      BitType mask = static_cast<BitType>(flag);

      _value ^= mask;
    }
  };

  template <typename T>
  class Flags {
    static_assert(!std::is_same_v<T, T>, "Underlying type must be an enum with a fixed width")
  };
}

#endif /* _LIB_FLAGS_HH */