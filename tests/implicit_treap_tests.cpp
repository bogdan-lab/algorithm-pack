#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

#include "algorithm_pack/implicit_treap.h"

using ::testing::ElementsAreArray;

TEST(ImplicitTreapTest, CreateEmpty) {
  alpa::ImplicitTreap<int> test;
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
  EXPECT_EQ(test.Begin(), test.End());
  alpa::ImplicitTreap<double> test2(35);
  EXPECT_TRUE(test2.Empty());
  EXPECT_EQ(test2.Size(), 0);
  EXPECT_EQ(test2.Begin(), test2.End());
}

TEST(ImplicitTreapTest, SingleElement) {
  alpa::ImplicitTreap<int> test(42);
  int val = 314;
  EXPECT_EQ(test.Insert(val, 0), val);
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), 1);
  ASSERT_NE(test.Begin(), test.End());
  EXPECT_EQ(*test.Begin(), val);
  *test.Begin() = 24;
  EXPECT_EQ(*test.Begin(), 24);
}

TEST(ImplicitTreapTest, PushBack) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(42);
  for (const auto& el : input) {
    test.Insert(el, test.Size());
  }
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), input.size());
  std::vector<int> result;
  result.reserve(test.Size());
  for (auto it = test.Begin(); it != test.End(); ++it) {
    result.push_back(*it);
  }
  EXPECT_THAT(result, ElementsAreArray(input));
}

TEST(ImplicitTreapTest, PushFront) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(42);
  for (const auto& el : input) {
    test.Insert(el, 0);
  }
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), input.size());
  std::vector<int> result;
  result.reserve(test.Size());
  for (auto it = test.Begin(); it != test.End(); ++it) {
    result.push_back(*it);
  }
  EXPECT_THAT(result, ElementsAreArray(input.rbegin(), input.rend()));
}

TEST(ImplicitTreapTest, InsertInside) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto create_treap = [](const std::vector<int>& input) {
    alpa::ImplicitTreap<int> res(314);
    for (const auto& el : input) {
      res.Insert(el, res.Size());
    }
    return res;
  };
  auto conv_to_vec = [](alpa::ImplicitTreap<int>& tr) {
    std::vector<int> res;
    res.reserve(tr.Size());
    for (auto it = tr.Begin(); it != tr.End(); ++it) {
      res.push_back(*it);
    }
    return res;
  };

  for (size_t i = 1; i < input.size(); ++i) {
    alpa::ImplicitTreap<int> test = create_treap(input);
    test.Insert(1024, i);
    std::vector<int> res = conv_to_vec(test);
    ASSERT_GT(res.size(), i);
    EXPECT_EQ(res[i], 1024) << " i = " << i;
  }
}

TEST(ImplicitTreapTest, IteratorConversion1) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x;
  for (const auto& el : input) {
    x.Insert(el, input.size());
  }
  alpa::ImplicitTreap<int>::Iterator begin = x.Begin();
  alpa::ImplicitTreap<int>::ConstIterator cbegin = begin;
  for (const auto& el : input) {
    EXPECT_EQ(el, *begin);
    EXPECT_EQ(*begin, *cbegin);
    ++begin;
    ++cbegin;
  }
}

TEST(ImplicitTreapTest, IteratorConversion2) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x;
  for (const auto& el : input) {
    x.Insert(el, input.size());
  }
  alpa::ImplicitTreap<int>::Iterator begin = x.Begin();
  alpa::ImplicitTreap<int>::ConstIterator cbegin = begin;
  for (const auto& el : input) {
    EXPECT_EQ(el, *begin);
    EXPECT_EQ(*begin++, *cbegin++);
  }
}

TEST(ImplicitTreapTest, IteratorConversion3) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x;
  for (const auto& el : input) {
    x.Insert(el, input.size());
  }
  alpa::ImplicitTreap<int>::Iterator last = std::prev(x.End());
  alpa::ImplicitTreap<int>::ConstIterator clast = std::prev(x.CEnd());
  for (auto it = input.rbegin(); it != input.rend(); ++it) {
    EXPECT_EQ(*it, *last);
    EXPECT_EQ(*last, *clast);
    --last;
    --clast;
  }
}

TEST(ImplicitTreapTest, IteratorConversion4) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x;
  for (const auto& el : input) {
    x.Insert(el, input.size());
  }
  alpa::ImplicitTreap<int>::Iterator last = std::prev(x.End());
  alpa::ImplicitTreap<int>::ConstIterator clast = std::prev(x.CEnd());
  for (auto it = input.rbegin(); it != input.rend(); ++it) {
    EXPECT_EQ(*it, *last);
    EXPECT_EQ(*last--, *clast--);
  }
}
