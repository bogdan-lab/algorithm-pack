#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "algorithm_pack/treap.h"

using ::testing::NotNull;

TEST(TreapTest, CreateEmpty) {
  Treap<int, std::string> test;
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
}

TEST(TreapTest, CreateAndFill) {
  std::vector<int> input{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  Treap<int, int> test;
  for (size_t i = 0; i < input.size(); ++i) {
    int input_val = input[i] * input[i];
    int* val = test.Insert(input[i], input_val);
    ASSERT_THAT(val, NotNull());
    EXPECT_EQ(*val, input_val);
    EXPECT_EQ(test.Size(), i + 1);
  }
}

// TODO Find in empty
// TODO Insert and find existing
// TODO Insert and find not existing
// TODO Insert and modify new and old values
// TODO Erase from empty
// TODO Erase existing to the end and check the then it is empty
// TODO Erase not existing and check that nothing is changed
