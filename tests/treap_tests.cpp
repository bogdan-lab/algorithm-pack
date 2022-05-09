#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

#include "algorithm_pack/treap.h"

using ::testing::IsEmpty;
using ::testing::IsNull;
using ::testing::Not;
using ::testing::NotNull;

TEST(TreapTest, CreateEmpty) {
  alpa::Treap<int, std::string> test;
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
}

TEST(TreapTest, CreateAndFill) {
  std::vector<int> input{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  alpa::Treap<int, int> test(/*seed=*/42);
  for (size_t i = 0; i < input.size(); ++i) {
    int input_val = input[i] * input[i];
    int* val = test.Insert(input[i], input_val);
    ASSERT_THAT(val, NotNull());
    EXPECT_EQ(*val, input_val);
    EXPECT_EQ(test.Size(), i + 1);
  }
}

TEST(TreapTest, FindInEmpty) {
  alpa::Treap<int, std::string> test;
  std::string* res = test.Find(5);
  EXPECT_THAT(res, IsNull());
}

TEST(TreapTest, FindExisting) {
  std::vector<int> input{0, 1, 2, 3, 4, 5, 6};
  std::vector<std::string> vals(input.size());
  std::transform(input.begin(), input.end(), vals.begin(),
                 [](int el) { return std::to_string(el); });
  do {
    std::next_permutation(vals.begin(), vals.end());
    alpa::Treap<int, std::string> test(/*seed=*/input.front());
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* res = test.Insert(input[i], vals[i]);
      ASSERT_THAT(res, NotNull());
      EXPECT_EQ(*res, vals[i]);
    }
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* res = test.Find(input[i]);
      ASSERT_THAT(res, NotNull());
      EXPECT_EQ(*res, vals[i]);
    }
  } while (std::next_permutation(input.begin(), input.end()));
}

TEST(TreapTest, FindNotExisting) {
  std::vector<int> input{0, 2, 4, 6, 8, 10, 12};
  std::vector<int> checks{-1, 1, 3, 5, 7, 9, 11, 13};
  std::vector<std::string> vals{"a", "b", "c", "d", "e", "f", "g"};
  do {
    std::next_permutation(vals.begin(), vals.end());
    alpa::Treap<int, std::string> test(/*seed=*/input.front());
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* res = test.Insert(input[i], vals[i]);
      ASSERT_THAT(res, NotNull());
      EXPECT_EQ(*res, vals[i]);
    }
    for (const auto& el : checks) {
      EXPECT_THAT(test.Find(el), IsNull());
    }
  } while (std::next_permutation(input.begin(), input.end()));
}

TEST(TreapTest, InsertAndModifyNewValue) {
  std::vector<int> input{0, 2, 4, 6, 8, 10, 12};
  std::vector<std::string> vals{"a", "b", "c", "d", "e", "f", "g"};
  do {
    std::next_permutation(vals.begin(), vals.end());
    alpa::Treap<int, std::string> test(/*seed=*/input.front());
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* val_ptr = test.Insert(input[i], "");
      ASSERT_THAT(val_ptr, NotNull());
      *val_ptr = vals[i];
    }
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* res = test.Find(input[i]);
      ASSERT_THAT(res, NotNull());
      EXPECT_EQ(*res, vals[i]);
    }
    EXPECT_EQ(test.Size(), input.size());
  } while (std::next_permutation(input.begin(), input.end()));
}

TEST(TreapTest, InsertAndModifyOldValue) {
  std::vector<int> input{0, 2, 4, 6, 8, 10, 12};
  std::vector<std::string> vals{"a", "b", "c", "d", "e", "f", "g"};
  alpa::Treap<int, std::string> test(/*seed=*/29);
  for (size_t i = 0; i < input.size(); ++i) {
    test.Insert(input[i], vals[i]);
  }
  size_t count;
  do {
    count++;
    std::string curr_val = std::to_string(count);
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* val_ptr = test.Insert(input[i], "");
      EXPECT_EQ(test.Size(), input.size());
      ASSERT_THAT(val_ptr, NotNull());
      EXPECT_THAT(*val_ptr, Not(IsEmpty()));
      *val_ptr = curr_val;
    }
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* res = test.Find(input[i]);
      ASSERT_THAT(res, NotNull());
      EXPECT_EQ(*res, curr_val);
    }
  } while (std::next_permutation(input.begin(), input.end()));
}

TEST(TreapTest, EraseFromEmpty) {
  alpa::Treap<int, int> test;
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
  EXPECT_FALSE(test.Erase(25));
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
  EXPECT_FALSE(test.Erase(0));
  EXPECT_FALSE(test.Erase(64));
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
}

TEST(TreapTest, EraseAllExisting) {
  std::vector<int> input{0, 1, 2, 3, 4, 5, 6};
  std::vector<int> check = input;
  do {
    alpa::Treap<int, int> test(/*seed=*/25);
    for (size_t i = 0; i < input.size(); ++i) {
      test.Insert(input[i], input[i] * input[i]);
    }
    size_t curr_size = input.size();
    EXPECT_EQ(test.Size(), curr_size);
    for (const auto& el : check) {
      EXPECT_TRUE(test.Erase(el));
      EXPECT_EQ(test.Size(), --curr_size);
    }
    EXPECT_TRUE(test.Empty());
  } while (std::next_permutation(input.begin(), input.end()));
}

TEST(TreapTest, EraseNotExisting) {
  std::vector<int> input{0, 2, 4, 6, 8, 10, 12};
  std::vector<int> check{-1, 1, 3, 5, 7, 9, 11, 13};
  do {
    alpa::Treap<int, int> test(/*seed=*/25);
    for (size_t i = 0; i < input.size(); ++i) {
      test.Insert(input[i], input[i] * input[i]);
    }
    for (const auto& el : check) {
      EXPECT_FALSE(test.Erase(el));
      EXPECT_EQ(test.Size(), input.size());
    }
  } while (std::next_permutation(input.begin(), input.end()));
}
