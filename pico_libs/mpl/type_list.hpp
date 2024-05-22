#pragma once
#include <cstdint>
#include <utility>

namespace xac::mpl {

template <typename... Args>
struct type_list {
  constexpr static uint64_t size = sizeof...(Args);
};

template <typename T, typename TList>
struct index_of;

template <typename T, template <typename...> class TList, typename... Tail>
struct index_of<T, TList<T, Tail...>> : std::integral_constant<uint64_t, 0> {};

template <typename T, template <typename...> class TList, typename Head, typename... Tail>
struct index_of<T, TList<Head, Tail...>> : std::integral_constant<uint64_t, index_of<T, TList<Tail...>>::value + 1> {};

template <typename T, typename TList>
inline constexpr uint64_t index_of_v = index_of<T, TList>::value;

template <uint64_t N, typename TList>  // T can be any type, include a template type
struct type_at;

// specialization for template class T
// here Head and Rest are prepared for T
// T must be a template type itself rather than a specialization of template type
// ...Args here are just for deducing the T and what are Args
// so we must use specialization to implement this
// without the raw template, we cannot do type deducing
template <template <typename...> class TList, typename Head, typename... Tail>
struct type_at<0, TList<Head, Tail...>> : std::common_type<Head> {};

template <uint64_t N, template <typename...> class TList, typename Head, typename... Tail>
struct type_at<N, TList<Head, Tail...>> : type_at<N - 1, TList<Tail...>> {
  static_assert(N < sizeof...(Tail) + 1, "Out of bound");
};

template <uint64_t N, typename TList>
using type_at_t = typename type_at<N, TList>::type;

namespace __detail {
template <template <typename...> class U, typename T>
struct rename;
template <template <typename...> class U, template <typename...> class T, typename... Args>
struct rename<U, T<Args...>> : std::common_type<U<Args...>> {};
}  // namespace __detail

template <template <typename...> class U, typename T>
using rename = typename __detail::rename<U, T>::type;

template <typename T, typename TList>
struct contains : std::false_type {};

template <typename T, template <typename...> class TList, typename... Rest>
struct contains<T, TList<T, Rest...>> : std::true_type {};

template <typename T, template <typename...> class TList, typename Head, typename... Rest>
struct contains<T, TList<Head, Rest...>> : contains<T, TList<Rest...>> {};

template <typename T, typename TList>
inline constexpr bool contains_v = contains<T, TList>::value;


}  // namespace xac::mpl