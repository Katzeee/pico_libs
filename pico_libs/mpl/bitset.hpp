#pragma once
#include <array>
#include <cstdint>
#include <numeric>

namespace xac::mpl {

template <uint64_t... I>
struct index_bits_uint64 : std::integral_constant<uint64_t, ((1 << I) | ...)> {};

template <uint64_t... I>
inline constexpr uint64_t index_bits_uint64_v = index_bits_uint64<I...>::value;

template <uint64_t N, uint64_t... I>
struct index_bits_str {
  static_assert(N < std::numeric_limits<uint64_t>::max(), "N is too big");
  static constexpr std::array<char, N + 1> make_value() {
    // +1 for '\0'
    std::array<char, N + 1> v;
    v.fill('0');
    v[N] = '\0';
    ((v[N - I - 1] = '1'), ...);
    return v;
  }

  static constexpr std::array<char, N + 1> value = index_bits_str<N, I...>::make_value();
};

template <uint64_t N, uint64_t... I>
inline constexpr std::array<char, N + 1> index_bits_str_v = index_bits_str<N, I...>::value;

}  // namespace xac::mpl