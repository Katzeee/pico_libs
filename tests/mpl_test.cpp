#include <experimental/type_traits>
#include <type_list.hpp>
// #include <utility>
using namespace xac;

#include "gtest/gtest.h"

template <typename T>
using detect_value = typename T::value_type;

TEST(MPL_TEST, TYPE_LIST) {
  using TestTypeList = mpl::type_list<int, bool, double>;
  using TestTuple = std::tuple<int, bool, double>;

  static_assert(!std::experimental::is_detected_v<detect_value, mpl::index_of<int, mpl::type_list<> > >);
  // static_assert(mpl::index_of<int, mpl::type_list<bool>>);  // won't compile
  static_assert(mpl::index_of_v<int, TestTypeList> == 0);
  static_assert(mpl::index_of_v<bool, TestTypeList> == 1);
  static_assert(mpl::index_of_v<double, TestTypeList> == 2);
  static_assert(mpl::index_of_v<int, TestTuple> == 0);
  static_assert(mpl::index_of_v<bool, TestTuple> == 1);
  static_assert(mpl::index_of_v<double, TestTuple> == 2);

  // static_assert(std::is_same_v<mpl::type_at_t<3, TestTypeList>, double>);  // won't compile
  static_assert(std::is_same_v<mpl::type_at_t<0, TestTypeList>, int>);
  static_assert(std::is_same_v<mpl::type_at_t<1, TestTypeList>, bool>);
  static_assert(std::is_same_v<mpl::type_at_t<2, TestTypeList>, double>);
  static_assert(std::is_same_v<mpl::type_at_t<0, TestTuple>, int>);
  static_assert(std::is_same_v<mpl::type_at_t<1, TestTuple>, bool>);
  static_assert(std::is_same_v<mpl::type_at_t<2, TestTuple>, double>);

  static_assert(mpl::contains_v<int, TestTypeList>);
  static_assert(mpl::contains_v<bool, TestTypeList>);
  static_assert(mpl::contains_v<double, TestTypeList>);
  static_assert(!mpl::contains_v<float, TestTypeList>);
  static_assert(mpl::contains_v<int, TestTuple>);
  static_assert(mpl::contains_v<bool, TestTuple>);
  static_assert(mpl::contains_v<double, TestTuple>);
  static_assert(!mpl::contains_v<float, TestTuple>);

  static_assert(std::is_same_v<mpl::rename<std::tuple, TestTypeList>, TestTuple>);
  static_assert(std::is_same_v<mpl::rename<mpl::type_list, TestTuple>, TestTypeList>);
}
