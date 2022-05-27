#include <gtest/gtest.h>

#include "algorithm_pack/implicit_treap.h"

TEST(ImplicitTreapTest, CreateEmpty) {
  alpa::ImplicitTreap<int> test;
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
  alpa::ImplicitTreap<double> test2;
  EXPECT_TRUE(test2.Empty());
  EXPECT_EQ(test2.Size(), 0);
}
