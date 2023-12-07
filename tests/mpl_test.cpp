#include <type_list.hpp>
using namespace xac::mpl;

#include "gtest/gtest.h"

TEST(MPL_TEST, TYPE_LIST) {
  const bool v = is_in_type_list<bool, type_list<bool, int>>::value;
  const bool v1 = is_in_type_list<bool, type_list<int>>::value;
  const bool v2 = is_in_type_list<bool, type_list<int, bool>>::value;
  EXPECT_EQ(v, true);
  EXPECT_EQ(v1, false);
  EXPECT_EQ(v2, true);
}