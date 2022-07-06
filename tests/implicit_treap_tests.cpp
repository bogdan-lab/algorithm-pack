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
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, PushFront) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(42);
  for (const auto& el : input) {
    test.Insert(el, 0);
  }
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), input.size());
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input.rbegin(), input.rend()));
}

TEST(ImplicitTreapTest, InsertInside) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (size_t i = 1; i < input.size(); ++i) {
    alpa::ImplicitTreap<int> test(input, /*seed=*/15);
    test.Insert(1024, i);
    std::vector<int> res(test.Begin(), test.End());
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
  alpa::ImplicitTreap<int> test2({1, 2, 3, 4, 5}, /*seed=*/54);
  test2 = std::move(test);
  EXPECT_TRUE(test2.Empty());
}

TEST(ImplicitTreapTest, Constructors2) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x(input, /*seed=*/15);
  alpa::ImplicitTreap<int> test(std::move(x));
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, Constructors3) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x(input, /*seed=*/15);
  alpa::ImplicitTreap<int> test(std::vector<int>{10, 20}, /*seed=*/15);
  test = std::move(x);
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, Constructor4) {
  // Move assignment to itself
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  test = std::move(test);
  EXPECT_EQ(test.Size(), input.size());
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, Constructors5) {
  alpa::ImplicitTreap<int> x;
  alpa::ImplicitTreap<int> test(x);
  EXPECT_TRUE(test.Empty());
  alpa::ImplicitTreap<int> test2({1, 2, 3, 4, 5}, /*seed=*/54);
  test2 = test;
  EXPECT_TRUE(test2.Empty());
}

TEST(ImplicitTreapTest, Constructors6) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x(input, /*seed=*/15);
  alpa::ImplicitTreap<int> test(x);
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
  EXPECT_THAT(std::vector<int>(x.Begin(), x.End()), ElementsAreArray(input));
  EXPECT_EQ(test.Size(), x.Size());
}

TEST(ImplicitTreapTest, Constructors7) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x(input, /*seed=*/15);
  alpa::ImplicitTreap<int> test(std::vector<int>{10, 20}, /*seed=*/15);
  test = x;
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
  EXPECT_THAT(std::vector<int>(x.Begin(), x.End()), ElementsAreArray(input));
  EXPECT_EQ(test.Size(), x.Size());
}

TEST(ImplicitTreapTest, Constructor8) {
  // Copy assignment to itself
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  test = test;
  EXPECT_EQ(test.Size(), input.size());
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
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
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, EraseFromBack) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  while (!input.empty()) {
    test.Erase(input.size() - 1);
    input.pop_back();
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, EraseFromFront) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/123);
  while (!input.empty()) {
    test.Erase(0);
    input.erase(input.begin());
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, EraseFromTheMiddle) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  while (!input.empty()) {
    size_t index = input.size() / 2;
    test.Erase(index);
    input.erase(input.begin() + static_cast<int>(index));
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, RotateFullVector) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> treap(input, /*seed=*/15);
  treap.Rotate(0, 1, treap.Size());
  std::rotate(input.begin(), input.begin() + 1, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(0, 4, treap.Size());
  std::rotate(input.begin(), input.begin() + 4, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(0, treap.Size() - 1, treap.Size());
  std::rotate(input.begin(), input.end() - 1, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, RotatePartOfTheVector) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> treap(input, /*seed=*/15);
  // Rotate in the beginning
  treap.Rotate(0, 1, 4);
  std::rotate(input.begin(), input.begin() + 1, input.begin() + 4);
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(0, 3, 4);
  std::rotate(input.begin(), input.begin() + 3, input.begin() + 4);
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(0, 2, 4);
  std::rotate(input.begin(), input.begin() + 2, input.begin() + 4);
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));

  // Rotate in the middle
  treap.Rotate(2, 3, 6);
  std::rotate(input.begin() + 2, input.begin() + 3, input.begin() + 6);
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(2, 5, 6);
  std::rotate(input.begin() + 2, input.begin() + 5, input.begin() + 6);
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(2, 4, 6);
  std::rotate(input.begin() + 2, input.begin() + 4, input.begin() + 6);
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  // Rotate at the end
  treap.Rotate(4, 5, treap.Size());
  std::rotate(input.begin() + 4, input.begin() + 5, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(4, 6, treap.Size());
  std::rotate(input.begin() + 4, input.begin() + 6, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(4, treap.Size() - 1, treap.Size());
  std::rotate(input.begin() + 4, input.end() - 1, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));

  treap.Rotate(0, 4, treap.Size());
  std::rotate(input.begin(), input.begin() + 4, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(0, treap.Size() - 1, treap.Size());
  std::rotate(input.begin(), input.end() - 1, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, RotateWithNoRotation) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> treap(input, /*seed=*/42);
  treap.Rotate(0, 0, treap.Size());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(5, 5, treap.Size());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(treap.Size() - 1, treap.Size() - 1, treap.Size());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(2, 2, 6);
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, RotateCornerCases) {
  {  // Rotate empty treap
    alpa::ImplicitTreap<int> treap;
    EXPECT_TRUE(treap.Empty());
    EXPECT_EQ(treap.Size(), 0);
    treap.Rotate(0, 0, 0);
    EXPECT_TRUE(treap.Empty());
    EXPECT_EQ(treap.Size(), 0);
  }
  {
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    alpa::ImplicitTreap<int> treap(input, /*seed=*/42);
    // Rotate empty subrange in the beginning
    treap.Rotate(0, 0, 0);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
    // Rotate empty subrange in the middle
    treap.Rotate(2, 2, 2);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
    // Rotate empty subrange in the end
    treap.Rotate(8, 8, 8);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
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
  EXPECT_EQ(++cres_end, cend);
}

TEST(ImplicitTreapTest, IteratorInvalidationRotate) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  auto it = test.Begin();
  auto cit = test.CBegin();
  auto end = test.End();
  auto cend = test.CEnd();
  test.Rotate(0, test.Size() - 2, test.Size());
  for (size_t i = 0; i < input.size() - 2; ++i) {
    EXPECT_EQ(*it++, input[i]);
    EXPECT_EQ(*cit++, input[i]);
  }
  EXPECT_EQ(it, end);
  EXPECT_EQ(cit, cend);
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
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
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
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
}

TEST(ImplicitTreapTest, IteratorCall) {
  std::vector<std::string> input{"one", "two", "three"};
  alpa::ImplicitTreap<std::string> test(input, /*seed=*/123);
  auto it = test.Begin();
  auto cit = test.CBegin();
  auto exp_it = input.begin();
  while (exp_it != input.end()) {
    EXPECT_EQ(it->find("e"), exp_it->find("e"));
    EXPECT_EQ(cit->find("e"), exp_it->find("e"));
    ++it;
    ++cit;
    ++exp_it;
  }
}

TEST(ImplicitTreapTest, Concatenate1) {
  {  // General case
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto mid_it = input.begin() + static_cast<int>(input.size() / 2);
    alpa::ImplicitTreap<int> lhs{{input.begin(), mid_it},
                                 /*seed=*/42};
    alpa::ImplicitTreap<int> rhs{{mid_it, input.end()},
                                 /*seed=*/123};
    lhs.Concatenate(std::move(rhs));
    EXPECT_EQ(lhs.Size(), input.size());
    EXPECT_THAT(std::vector<int>(lhs.Begin(), lhs.End()),
                ElementsAreArray(input));
  }
  {  // Concatenate empty
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    alpa::ImplicitTreap<int> lhs{input, /*seed=*/42};
    alpa::ImplicitTreap<int> rhs;
    lhs.Concatenate(std::move(rhs));
    EXPECT_EQ(lhs.Size(), input.size());
    EXPECT_THAT(std::vector<int>(lhs.Begin(), lhs.End()),
                ElementsAreArray(input));
  }
  {  // Concatenate to empty
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    alpa::ImplicitTreap<int> lhs;
    alpa::ImplicitTreap<int> rhs{input, /*seed=*/42};
    lhs.Concatenate(std::move(rhs));
    EXPECT_EQ(lhs.Size(), input.size());
    EXPECT_THAT(std::vector<int>(lhs.Begin(), lhs.End()),
                ElementsAreArray(input));
  }
}

TEST(ImplicitTreapTest, Concatenate2) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test;
  test.Insert(0, /*pos=*/0);
  auto it = test.Begin();
  auto cit = test.CBegin();
  auto end = test.End();
  auto cend = test.CEnd();
  int prev_val = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    test.Concatenate(alpa::ImplicitTreap<int>({input[i]}, /*seed=*/314));
    ++it;
    ++cit;
    EXPECT_EQ(*it, input[i]);
    EXPECT_EQ(*cit, input[i]);
    --it;
    --cit;
    EXPECT_EQ(*it, prev_val);
    EXPECT_EQ(*cit, prev_val);
    ++it;
    ++cit;
    prev_val = input[i];
  }
  EXPECT_EQ(test.End(), end);
  EXPECT_EQ(test.CEnd(), cend);
}

TEST(ImplicitTreapTest, RandomAccessIteratorShifts1) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
  alpa::ImplicitTreap<int> test(input, /*seed=*/15);
  {
    auto it = test.Begin();
    auto cit = test.CBegin();
    EXPECT_EQ(*(it + 1), input[1]);
    EXPECT_EQ(*(cit + 1), input[1]);
    EXPECT_EQ(*(1 + it), input[1]);
    EXPECT_EQ(*(1 + cit), input[1]);
    EXPECT_EQ(*(it + 0), input[0]);
    EXPECT_EQ(*(cit + 0), input[0]);
    EXPECT_EQ(*(0 + it), input[0]);
    EXPECT_EQ(*(0 + cit), input[0]);
    EXPECT_EQ(*(it + 3), input[3]);
    EXPECT_EQ(*(cit + 3), input[3]);
    EXPECT_EQ(*(3 + it), input[3]);
    EXPECT_EQ(*(3 + cit), input[3]);
    int last_index = static_cast<int>(input.size()) - 1;
    EXPECT_EQ(*(it + last_index), input.back());
    EXPECT_EQ(*(cit + last_index), input.back());
    EXPECT_EQ(*(last_index + it), input.back());
    EXPECT_EQ(*(last_index + cit), input.back());
    EXPECT_EQ(it + static_cast<int>(input.size()), test.End());
    EXPECT_EQ(cit + static_cast<int>(input.size()), test.CEnd());
    EXPECT_EQ(static_cast<int>(input.size()) + it, test.End());
    EXPECT_EQ(static_cast<int>(input.size()) + cit, test.CEnd());
  }
  {
    auto it = test.End();
    auto cit = test.CEnd();
    EXPECT_EQ(*(it - 1), input.back());
    EXPECT_EQ(*(cit - 1), input.back());
    EXPECT_EQ(*(it - 3), input[5]);
    EXPECT_EQ(*(cit - 3), input[5]);
    int last_index = static_cast<int>(input.size()) - 1;
    EXPECT_EQ(*(it - last_index), input[1]);
    EXPECT_EQ(*(cit - last_index), input[1]);
    EXPECT_EQ(*(it - static_cast<int>(input.size())), input.front());
    EXPECT_EQ(*(cit - static_cast<int>(input.size())), input.front());
  }
  {
    auto it = test.Begin() + 3;
    auto cit = test.CBegin() + 3;
    EXPECT_EQ(*(it - 0), input[3]);
    EXPECT_EQ(*(cit - 0), input[3]);
    EXPECT_EQ(*(it + 0), input[3]);
    EXPECT_EQ(*(cit + 0), input[3]);
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

TEST(ImplicitTreapTest, RandomAccessIteratorShifts2) {
  // test jump from different points in the tree to some other different points
  // in the tree
  std::vector<int> input(10000);
  std::iota(input.begin(), input.end(), 0);
  alpa::ImplicitTreap<int> test(input, /*seed=*/56);
  std::mt19937 rnd(42);
  std::uniform_int_distribution<int> dist(0,
                                          static_cast<int>(input.size()) - 1);
  int count = 500;
  auto it = test.Begin();
  auto cit = test.CBegin();
  while (count--) {
    int index_from = dist(rnd);
    int index_to = dist(rnd);
    int shift = index_to - index_from;
    EXPECT_EQ(*((it + index_from) + shift),
              input[static_cast<size_t>(index_to)]);
    EXPECT_EQ(*((cit + index_from) + shift),
              input[static_cast<size_t>(index_to)]);
  }
}

TEST(ImplicitTreapTest, RandomAccessIteratorDifference) {
  std::vector<int> input(10000);
  std::iota(input.begin(), input.end(), 0);
  alpa::ImplicitTreap<int> test(input, /*seed=*/56);
  std::mt19937 rnd(42);
  std::uniform_int_distribution<int> dist(0,
                                          static_cast<int>(input.size()) - 1);
  int count = 500;
  auto it = test.Begin();
  auto cit = test.CBegin();
  while (count--) {
    int index_from = dist(rnd);
    int index_to = dist(rnd);
    int expected = index_to - index_from;
    auto it_from = it + index_from;
    auto it_to = it + index_to;
    auto cit_from = cit + index_from;
    auto cit_to = cit + index_to;
    EXPECT_EQ(it_to - it_from, expected);
    EXPECT_EQ(cit_to - cit_from, expected);
    EXPECT_EQ(cit_to - it_from, expected);
    EXPECT_EQ(it_to - cit_from, expected);
  }
  EXPECT_EQ(test.End() - it, test.Size());
  EXPECT_EQ(test.End() - cit, test.Size());
  EXPECT_EQ(test.CEnd() - cit, test.Size());
  EXPECT_EQ(test.End() - cit, test.Size());
}

TEST(ImplicitTreapTest, Extract1) {
  {  // Extract entire range
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    alpa::ImplicitTreap<int> test(input, /*seed=*/519);
    alpa::ImplicitTreap<int> res = test.Extract(0, test.Size());
    EXPECT_EQ(test.Size(), 0);
    EXPECT_TRUE(test.Empty());
    EXPECT_EQ(res.Size(), input.size());
    EXPECT_FALSE(res.Empty());
    EXPECT_THAT(std::vector<int>(res.Begin(), res.End()),
                ElementsAreArray(input));
  }
  {  // Extract empty range
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    alpa::ImplicitTreap<int> test(input, /*seed=*/519);
    alpa::ImplicitTreap<int> res = test.Extract(5, 5);
    EXPECT_EQ(res.Size(), 0);
    EXPECT_TRUE(res.Empty());
    EXPECT_EQ(test.Size(), input.size());
    EXPECT_FALSE(test.Empty());
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
  {  // Extract single element
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    alpa::ImplicitTreap<int> test(input, /*seed=*/519);
    alpa::ImplicitTreap<int> res = test.Extract(5, 6);
    EXPECT_EQ(res.Size(), 1);
    EXPECT_FALSE(res.Empty());
    EXPECT_EQ(test.Size(), input.size() - 1);
    EXPECT_FALSE(test.Empty());
    EXPECT_THAT(std::vector<int>(res.Begin(), res.End()), ElementsAre(6));
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAre(1, 2, 3, 4, 5, 7, 8, 9));
  }
  {  // Extract empty from the empty
    alpa::ImplicitTreap<int> test(/*seed=*/519);
    alpa::ImplicitTreap<int> res = test.Extract(0, 0);
    EXPECT_EQ(test.Size(), 0);
    EXPECT_TRUE(test.Empty());
    EXPECT_EQ(res.Size(), 0);
    EXPECT_TRUE(res.Empty());
  }
  {  // Extract from the beginning
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    alpa::ImplicitTreap<int> test(input, /*seed=*/519);
    alpa::ImplicitTreap<int> start = test.Extract(0, 4);
    EXPECT_EQ(start.Size(), 4);
    EXPECT_EQ(test.Size(), input.size() - 4);
    EXPECT_THAT(std::vector<int>(start.Begin(), start.End()),
                ElementsAreArray(input.begin(), input.begin() + 4));
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input.begin() + 4, input.end()));
  }
  {  // Extract from the end
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    alpa::ImplicitTreap<int> test(input, /*seed=*/519);
    alpa::ImplicitTreap<int> end = test.Extract(6, input.size());
    EXPECT_EQ(end.Size(), input.size() - 6);
    EXPECT_FALSE(end.Empty());
    EXPECT_EQ(test.Size(), 6);
    EXPECT_FALSE(test.Empty());
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input.begin(), input.begin() + 6));
    EXPECT_THAT(std::vector<int>(end.Begin(), end.End()),
                ElementsAreArray(input.begin() + 6, input.end()));
  }
  {  // Extract from the middle
    std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    alpa::ImplicitTreap<int> test(input, /*seed=*/519);
    alpa::ImplicitTreap<int> middle = test.Extract(4, 7);
    EXPECT_EQ(middle.Size(), 3);
    EXPECT_FALSE(middle.Empty());
    EXPECT_EQ(test.Size(), input.size() - 3);
    EXPECT_FALSE(test.Empty());
    EXPECT_THAT(std::vector<int>(middle.Begin(), middle.End()),
                ElementsAreArray(input.begin() + 4, input.begin() + 7));
    std::vector<int> reminder(input.begin(), input.begin() + 4);
    reminder.insert(reminder.end(), input.begin() + 7, input.end());
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(reminder));
  }
}

TEST(ImplicitTreapTest, Extract2) {
  // testing iterator invalidation
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> expected{1, 2, 3, 4, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/57);
  auto it = test.Begin();
  auto cit = test.CBegin();
  auto end = test.End();
  auto cend = test.CEnd();
  alpa::ImplicitTreap<int> extracted = test.Extract(4, 7);
  for (const auto& el : expected) {
    EXPECT_EQ(*it++, el);
    EXPECT_EQ(*cit++, el);
  }
  EXPECT_EQ(it, end);
  EXPECT_EQ(cit, cend);
}

TEST(ImplicitTreapTest, StlMethodCalls) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/45);
  std::vector<int> result(test.Begin(), test.End());
  EXPECT_THAT(result, ElementsAreArray(input));
  std::vector<int> cresult(test.CBegin(), test.CEnd());
  EXPECT_THAT(cresult, ElementsAreArray(input));
  auto cit = std::lower_bound(test.CBegin(), test.CEnd(), 6);
  auto it = std::lower_bound(test.Begin(), test.End(), 6);
  ASSERT_NE(cit, test.CEnd());
  ASSERT_NE(it, test.End());
  EXPECT_EQ(*cit, 6);
  EXPECT_EQ(*std::next(cit), 7);
  EXPECT_EQ(*it, 6);
  EXPECT_EQ(*std::prev(it), 5);
}

TEST(ImplicitTreapTest, SwapTest) {
  std::vector<int> input_1{1, 2, 3, 4, 5, 6};
  std::vector<int> input_2{6, 5, 4, 3, 2, 1};
  alpa::ImplicitTreap<int> lhs{input_1, /*seed=*/1};
  alpa::ImplicitTreap<int> rhs{input_2, /*seed=*/2};
  lhs.Swap(rhs);
  EXPECT_THAT(std::vector<int>(lhs.Begin(), lhs.End()),
              ElementsAreArray(input_2));
  EXPECT_THAT(std::vector<int>(rhs.Begin(), rhs.End()),
              ElementsAreArray(input_1));
  rhs.Swap(rhs);
  EXPECT_THAT(std::vector<int>(rhs.Begin(), rhs.End()),
              ElementsAreArray(input_1));
}

TEST(ImplicitTreapTest, ClearTest) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
  alpa::ImplicitTreap<int> test(input, /*seed=*/123);
  test.Clear();
  EXPECT_EQ(test.Size(), 0);
  EXPECT_TRUE(test.Empty());
  test.Insert(1, /*pos=*/0);
  test.Insert(12, /*pos=*/0);
  test.Insert(123, /*pos=*/0);
  alpa::ImplicitTreap<int> dest(std::move(test));
  test.Clear();
  EXPECT_EQ(test.Size(), 0);
  EXPECT_TRUE(test.Empty());
}