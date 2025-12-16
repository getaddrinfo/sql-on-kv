#ifndef _KV_INTERNAL_TUPLE_HH
#define _KV_INTERNAL_TUPLE_HH

#include <vector>
#include <string>

#include <inttypes.h>

namespace kv::detail::tuple {
  constexpr uint8_t null_value = 0x00;
  constexpr uint8_t byte_string = 0x01;
  constexpr uint8_t unicode_string = 0x02;
  constexpr uint8_t nested_tuple = 0x05;

  constexpr uint8_t negative_8_byte_num = 0x0c;
  constexpr uint8_t negative_4_byte_num = 0x10;
  constexpr uint8_t negative_2_byte_num = 0x12;
  constexpr uint8_t negative_1_byte_num = 0x13;
  constexpr uint8_t zero_num = 0x14;
  constexpr uint8_t positive_1_byte_num = 0x15;
  constexpr uint8_t positive_2_byte_num = 0x16;
  constexpr uint8_t positive_4_byte_num = 0x18;
  constexpr uint8_t positive_8_byte_num = 0x1c;

  constexpr uint8_t ieee_float = 0x20;
  constexpr uint8_t ieee_double = 0x21;
  constexpr uint8_t ieee_long_double = 0x22;  

  constexpr uint8_t false_value = 0x26;
  constexpr uint8_t true_value = 0x27;

  constexpr uint8_t rfc_4122_uuid = 0x30;
  constexpr uint8_t identifier = 0x31;
  constexpr uint8_t version_stamp_80bit = 0x32;
  constexpr uint8_t version_stamp_96bit = 0x33;

  constexpr uint8_t end_of_tuple = 0xf0;
  constexpr uint8_t escape = 0xff;

  template <typename T>
  struct IntegerIdentifier { static const uint8_t pos; static const uint8_t neg; };

  template <>
  const uint8_t IntegerIdentifier<uint8_t>::pos = positive_1_byte_num;
  const uint8_t IntegerIdentifier<uint16_t>::pos = positive_2_byte_num;
  const uint8_t IntegerIdentifier<uint32_t>::pos = positive_4_byte_num;
  const uint8_t IntegerIdentifier<uint64_t>::pos = positive_8_byte_num;
  const uint8_t IntegerIdentifier<int8_t>::pos = positive_1_byte_num;
  const uint8_t IntegerIdentifier<int8_t>::neg = negative_1_byte_num;
  const uint8_t IntegerIdentifier<int16_t>::pos = positive_2_byte_num;
  const uint8_t IntegerIdentifier<int16_t>::neg = negative_2_byte_num;
  const uint8_t IntegerIdentifier<int32_t>::pos = positive_4_byte_num;
  const uint8_t IntegerIdentifier<int32_t>::neg = negative_4_byte_num;
  const uint8_t IntegerIdentifier<int64_t>::pos = positive_8_byte_num;
  const uint8_t IntegerIdentifier<int64_t>::neg = negative_8_byte_num;

  template <typename T>
  struct FloatIdentifier { static const uint8_t value; };

  template <>
  const uint8_t FloatIdentifier<float>::value = ieee_float;
  const uint8_t FloatIdentifier<double>::value = ieee_double;
  const uint8_t FloatIdentifier<long double>::value = ieee_long_double;

  template <typename T>
  uint8_t tag(T value) {
    // A quick branch for `T` not being able to
    // contain negative values (unsigned types).
    if constexpr (std::numeric_limits<T>::min() == 0) {
      return value == 0 
        ? zero_num 
        : IntegerIdentifier<T>::pos;
    }
    
    if (value == 0) {
      return zero_num;
    }

    return value < 0 
      ? IntegerIdentifier<T>::neg 
      : IntegerIdentifier<T>::pos;
  }

  template <typename T>
  class packer {
    std::vector<uint8_t> _state;

    void _push(uint8_t);

  public:
    void push_null();
    template <
      typename T,
      typename std::enable_if<std::is_integral_v<T>>::type = 0
    >
    void push(T value) {
      _push(tag<T>(value));

      // TODO: Add the value
    }

    void push_id(uint64_t);


    void push(float);
    void push(double);
    void push(bool);

    void push_uuid(std::string_view);
    void push_version_stamp(std::string_view);
    void push_byte_string(std::string_view);
    void push_unicode_string(std::string_view);
    void push_length_prefixed_byte_string(std::string_view);
    void push(const packer&);

    template <typename T>
    operator<<(T value) {
      push(value);
    }

    std::vector<uint8_t> finish(); 
  };
}

#endif /* _KV_INTERNAL_TUPLE_HH */