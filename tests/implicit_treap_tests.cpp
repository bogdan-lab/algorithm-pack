#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <utility>
#include <vector>

#include "algorithm_pack/implicit_treap.h"

using ::testing::ElementsAre;
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
  EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
}

TEST(ImplicitTreapTest, PushFront) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(42);
  for (const auto& el : input) {
    test.Insert(el, 0);
  }
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), input.size());
  EXPECT_THAT(test.ConvertToVector(),
              ElementsAreArray(input.rbegin(), input.rend()));
}

TEST(ImplicitTreapTest, InsertInside) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (size_t i = 1; i < input.size(); ++i) {
    alpa::ImplicitTreap<int> test(input, /*seed=*/15);
    test.Insert(1024, i);
    std::vector<int> res = test.ConvertToVector();
    ASSERT_GT(res.size(), i);
    EXPECT_EQ(res[i], 1024) << " i = " << i;
  }
}

TEST(ImplicitTreapTest, IteratorWalk1) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x(input, /*seed=*/15);
  alpa::ImplicitTreap<int>::Iterator begin = x.Begin();
  alpa::ImplicitTreap<int>::ConstIterator cbegin = begin;
  for (const auto& el : input) {
    EXPECT_EQ(el, *begin);
    EXPECT_EQ(*begin, *cbegin);
    ++begin;
    ++cbegin;
  }
}

TEST(ImplicitTreapTest, IteratorWalk2) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x(input, /*seed=*/15);
  alpa::ImplicitTreap<int>::Iterator begin = x.Begin();
  alpa::ImplicitTreap<int>::ConstIterator cbegin = x.CBegin();
  for (const auto& el : input) {
    EXPECT_EQ(el, *begin);
    EXPECT_EQ(*begin++, *cbegin++);
  }
}

TEST(ImplicitTreapTest, IteratorWalk3) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x(input, /*seed=*/15);
  alpa::ImplicitTreap<int>::Iterator last = --x.End();
  alpa::ImplicitTreap<int>::ConstIterator clast = --x.CEnd();
  for (auto it = input.rbegin(); it != input.rend(); ++it) {
    EXPECT_EQ(*it, *last);
    EXPECT_EQ(*last, *clast);
    --last;
    --clast;
  }
}

TEST(ImplicitTreapTest, IteratorWalk4) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x(input, /*seed=*/15);
  alpa::ImplicitTreap<int>::Iterator last = --x.End();
  alpa::ImplicitTreap<int>::ConstIterator clast = --x.CEnd();
  for (auto it = input.rbegin(); it != input.rend(); ++it) {
    EXPECT_EQ(*it, *last);
    EXPECT_EQ(*last--, *clast--);
  }
}

TEST(ImplicitTreapTest, Constructors1) {
  alpa::ImplicitTreap<int> x;
  alpa::ImplicitTreap<int> test(std::move(x));
  EXPECT_TRUE(test.Empty());
  alpa::ImplicitTreap<int> test2 = std::move(test);
  EXPECT_TRUE(test2.Empty());
}

TEST(ImplicitTreapTest, Constructors2) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x(input, /*seed=*/15);
  alpa::ImplicitTreap<int> test(std::move(x));
  EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
}

TEST(ImplicitTreapTest, Constructors3) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x(input, /*seed=*/15);
  alpa::ImplicitTreap<int> test(std::vector<int>{10, 20}, /*seed=*/15);
  test = std::move(x);
  EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
}

TEST(ImplicitTreapTest, ElementAccess1) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  const alpa::ImplicitTreap<int> ctest(input, /*seed=*/15);
  for (size_t i = 0; i < input.size(); ++i) {
    EXPECT_EQ(test[i], input[i]);
    EXPECT_EQ(ctest[i], input[i]);
  }
}

TEST(ImplicitTreapTest, ElementAccess2) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  int special_val = 22564;
  for (size_t i = 0; i < test.Size(); ++i) {
    if (i % 2) {
      test[i] = special_val;
      input[i] = special_val;
    }
  }
  EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
}

TEST(ImplicitTreapTest, EraseFromBack) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  while (!input.empty()) {
    test.Erase(input.size() - 1);
    input.pop_back();
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, EraseFromFront) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/123);
  while (!input.empty()) {
    test.Erase(0);
    input.erase(input.begin());
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, EraseFromTheMiddle) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  while (!input.empty()) {
    size_t index = input.size() / 2;
    test.Erase(index);
    input.erase(input.begin() + index);
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, RotatePositiveDirection) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  for (int i = 1; i < input.size(); ++i) {
    test.Rotate(i);
    std::rotate(input.begin(), input.end() - i, input.end());
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
}

TEST(ImplicitTreapTest, RotateNegativeDirection) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  for (int i = 1; i < input.size(); ++i) {
    test.Rotate(-i);
    std::rotate(input.begin(), input.begin() + i, input.end());
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
}

TEST(ImplicitTreapTest, RotateCountlargerThanSize) {
  std::vector<int> input{1, 2, 3, 4};
  {
    alpa::ImplicitTreap<int> test(input, /*seed=*/15);
    test.Rotate(5);
    EXPECT_THAT(test.ConvertToVector(), ElementsAre(4, 1, 2, 3));
  }
  {
    alpa::ImplicitTreap<int> test(input, /*seed=*/15);
    test.Rotate(7);
    EXPECT_THAT(test.ConvertToVector(), ElementsAre(2, 3, 4, 1));
  }
  {
    alpa::ImplicitTreap<int> test(input, /*seed=*/15);
    test.Rotate(-5);
    EXPECT_THAT(test.ConvertToVector(), ElementsAre(2, 3, 4, 1));
  }
  {
    alpa::ImplicitTreap<int> test(input, /*seed=*/15);
    test.Rotate(-7);
    EXPECT_THAT(test.ConvertToVector(), ElementsAre(4, 1, 2, 3));
  }
  {
    alpa::ImplicitTreap<int> test(input, /*seed=*/15);
    test.Rotate(0);
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
}

TEST(ImplicitTreapTest, IteratorInvalidationInsert) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
  alpa::ImplicitTreap<int> test;
  std::vector<alpa::ImplicitTreap<int>::Iterator> iterators;
  iterators.reserve(input.size());
  std::vector<alpa::ImplicitTreap<int>::ConstIterator> const_iterators;
  const_iterators.reserve(input.size());
  auto end = test.End();
  auto cend = test.CEnd();
  for (size_t i = 0; i < input.size(); ++i) {
    test.Insert(input[i], test.Size());
    if (iterators.empty()) {
      iterators.push_back(test.Begin());
      const_iterators.push_back(test.CBegin());
    } else {
      auto prev = iterators.back();
      iterators.push_back(++prev);
      auto cprev = const_iterators.back();
      const_iterators.push_back(++cprev);
    }
  }
  for (size_t i = 0; i < input.size(); ++i) {
    EXPECT_EQ(input[i], *iterators[i]);
    EXPECT_EQ(input[i], *const_iterators[i]);
  }
  auto res_end = iterators.back();
  auto cres_end = const_iterators.back();
  EXPECT_EQ(++res_end, end);
  EXPECT_EQ(++cres_end, end);
}

TEST(ImplicitTreapTest, IteratorInvalidationRotate) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  {
    alpa::ImplicitTreap<int> test(input, /*seed=*/15);
    auto it = test.Begin();
    auto cit = test.CBegin();
    auto end = test.End();
    auto cend = test.CEnd();
    test.Rotate(2);
    for (size_t i = 0; i < input.size() - 2; ++i) {
      EXPECT_EQ(*it++, input[i]);
      EXPECT_EQ(*cit++, input[i]);
    }
    EXPECT_EQ(it, end);
    EXPECT_EQ(cit, cend);
  }
  {
    alpa::ImplicitTreap<int> test(input, /*seed=*/15);
    auto it = test.Begin();
    auto cit = test.CBegin();
    auto end = test.End();
    auto cend = test.CEnd();
    test.Rotate(-2);
    for (size_t i = 0; i < 2; ++i) {
      EXPECT_EQ(*it++, input[i]);
      EXPECT_EQ(*cit++, input[i]);
    }
    EXPECT_EQ(it, end);
    EXPECT_EQ(cit, cend);
  }
}

TEST(ImplicitTreapTest, IteratorInvalidationErase) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  auto it = test.Begin();
  auto cit = test.CBegin();
  auto end = test.End();
  auto cend = test.CEnd();
  size_t count = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    if (!(input[i] % 2)) {
      test.Erase(i - count);
      ++count;
    }
  }
  for (size_t i = 0; i < input.size(); ++i) {
    if (input[i] % 2) {
      EXPECT_EQ(*it++, input[i]);
      EXPECT_EQ(*cit++, input[i]);
    }
  }
  EXPECT_EQ(it, end);
  EXPECT_EQ(cit, cend);
}

TEST(ImplicitTreapTest, ConstructFromVector) {
  {
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
    alpa::ImplicitTreap<int> test(input, /*seed=*/15);
    EXPECT_FALSE(test.Empty());
    EXPECT_EQ(test.Size(), input.size());
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
  {
    std::vector<int> input{};
    alpa::ImplicitTreap<int> test(input, /*seed=*/42);
    EXPECT_TRUE(test.Empty());
    EXPECT_EQ(test.Size(), 0);
  }
  {
    std::vector<int> input{25};
    alpa::ImplicitTreap<int> test(input, /*seed=*/42);
    EXPECT_FALSE(test.Empty());
    EXPECT_EQ(test.Size(), 1);
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
}

TEST(ImplicitTreapTest, RandomAccessIteratorShifts) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  {
    auto it = test.Begin();
    auto cit = test.CBegin();
    EXPECT_EQ(*(it + 1), input[1]);
    EXPECT_EQ(*(cit + 1), input[1]);
    EXPECT_EQ(*(it + 3), input[3]);
    EXPECT_EQ(*(cit + 3), input[3]);
    EXPECT_EQ(*(it + input.size() - 1), input.back());
    EXPECT_EQ(*(cit + input.size() - 1), input.back());
    EXPECT_EQ(*(it + input.size()), test.End());
    EXPECT_EQ(*(cit + input.size()), test.CEnd());
  }
  {
    auto it = test.End();
    auto cit = test.CEnd();
    EXPECT_EQ(*(it - 1), input.back());
    EXPECT_EQ(*(cit - 1), input.back());
    EXPECT_EQ(*(it - 3), input[5]);
    EXPECT_EQ(*(cit - 3), input[5]);
    EXPECT_EQ(*(it - input.size() + 1), input[1]);
    EXPECT_EQ(*(cit - input.size() + 1), input[1]);
    EXPECT_EQ(*(it - input.size()), input.front());
    EXPECT_EQ(*(cit - input.size()), input.front());
  }
  {
    auto it = test.Begin() + 3;
    auto cit = test.CBegin() + 3;
    EXPECT_EQ(*(it - 1), input[2]);
    EXPECT_EQ(*(cit - 1), input[2]);
    EXPECT_EQ(*(it + 1), input[4]);
    EXPECT_EQ(*(cit + 1), input[4]);
    EXPECT_EQ(*(it - 3), input[0]);
    EXPECT_EQ(*(cit - 3), input[0]);
    EXPECT_EQ(*(it + 3), input[6]);
    EXPECT_EQ(*(cit + 3), input[6]);
  }
}