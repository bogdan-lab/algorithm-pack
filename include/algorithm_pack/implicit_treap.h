#ifndef INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H
#define INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H

#include <random>

namespace alpa {
/**
 * @brief Realization of a treap with an implicit key. Basically the structure
 * is an node based array with improved asymptotic complexity of some
 * operations. In current realization, insertion, deletion and rotation have
 * O(log n) complexity, where n is container size.
 */
template <typename T>
class ImplicitTreap {
 public:
  /**@brief Creates an empty treap.*/
  ImplicitTreap() = default;
  /**@brief Creates an empty treap with the given seed set in random generator*/
  ImplicitTreap(uint64_t seed) : rnd_(seed) {}

  ~ImplicitTreap() {
    DeleteTree(root_);
    root_ = nullptr;
  }
  /**
   * @brief Sets seed of the random generator associated with current tree.
   * Random generator is used for creating priorities.
   */
  void SetSeed(uint64_t seed) { rnd_.seed(seed); }

  /**@brief Returns true if the container is empty*/
  bool Empty() const { return !root_; }
  /**@brief Gets the number of elements in the container.*/
  size_t Size() const { return size_; }
  /**
   * @brief Inserts the given value into given position.
   * TODO What if the given position is out of range?
   * TODO Also return pointer to the value?
   */
  void Insert(const T& value, int pos);
  /**
   * @brief Deletes the element from the container, which is stored in the given
   * position.
   * TODO What if the given position is out of range?
   */
  void Erase(int pos);
  /**
   * @brief Performs cyclic rotation of the container on the given position
   * number to the right. If the given number is less than 0, the cyclic
   * rotation to the left will be performed.
   */
  void Rotate(int count);

 private:
  struct Node {
    Node(const T& val, uint64_t p) : priority(p), value(val) {}

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    size_t tree_size = 0;
    uint64_t priority = 0;
    T value;
  };

  static void DeleteTree(Node* root) {
    if (!root) return;
    DeleteTree(root->left);
    DeleteTree(root->right);
    delete root;
  }

  Node* root_ = nullptr;
  std::mt19937_64 rnd_;
  size_t size_ = 0;
};

// TODO Need to have iterators here!

}  // namespace alpa

#endif  // INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H
