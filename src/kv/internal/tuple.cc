#include "tuple.hh"

#include <span>

namespace kv::detail::tuple {
  void Packer::push_byte_string(std::span<uint8_t> view) {
    _push(byte_string);
    for(const uint8_t val : view) {
      if (val == 0x00) {
        _push(0x00);
        _push(0x0f);
        continue;
      } 

      _push(val);
    }

    _push(0x00);
  }

  void Packer::push(bool c) {
    _push(c ? true_value : false_value);
  }

  void Packer::push_uuid(std::string_view uuid) {
    _push(rfc_4122_uuid);

    for(const char c : uuid) {
      _push(c);
    }
  } 

  void Packer::push(float v) {
    _push(ieee_float);

    std::span<uint8_t, sizeof(float)> bytes{
      reinterpret_cast<uint8_t*>(&v),
      sizeof(float)
    };

    if (bytes[0] & 0x80 != 0) {
      for(size_t i = 0; i < sizeof(float); i++) {
        bytes[i] ^= 0xFF;
      }
    } else {
      bytes[0] ^= 0x80;
    }

    _push(bytes);
  }

  std::vector<uint8_t> Packer::finish() {
    _state.push_back(end_of_tuple);
    return _state;
  }

  void Packer::_push(uint8_t v) {
    _state.push_back(v);
  }

  void Packer::_push(std::span<uint8_t> vs) {
    for(const uint8_t v : vs) {
      _state.push_back(v);
    }
  }
}