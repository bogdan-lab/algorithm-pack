#ifndef INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H
#define INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H

#include <cassert>
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
  struct Node;

 public:
  class Iterator {
   public:
    friend class ImplicitTreap;
    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
      return lhs.curr_node_ == rhs.curr_node_;
    }
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
      return lhs.curr_node_ != rhs.curr_node_;
    }
    /**@brief Creates empty iterator*/
    Iterator() = default;
    Iterator& operator++() {
      curr_node_ = GetNext();
      return *this;
    }
    Iterator operator++(int) {
      Node* old_node = curr_node_;
      curr_node_ = GetNext();
      return {old_node};
    }
    Iterator& operator--() {
      curr_node_ = GetPrev();
      return *this;
    }
    Iterator operator--(int) {
      Node* old_node = curr_node_;
      curr_node_ = GetPrev();
      return {old_node};
    }
    T& operator*() { return curr_node_->value; }

   private:
    explicit Iterator(Node* node) : curr_node_(node) {}
    /**Returns the next node in the tree*/
    Node* GetNext() const {
      // if curr_node_ here is nullptr we are searching next from end or from
      // default constructed iterator. User should not do this.
      assert(curr_node_);
      Node* right = curr_node_->right;
      if (!right) return right;
      while (right->left) {
        right = right->left;
      }
      return right;
    }
    /**Returns the previous node in the tree*/
    Node* GetPrev() const {
      // Similarly to GetNext, this should not be called with empty curr_node_
      assert(curr_node_);
      Node* left = curr_node_->left;
      if (!left) return left;
      while (left->right) {
        left = left->right;
      }
      return left;
    }

    Node* curr_node_ = nullptr;
  };

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
  /**
   * @brief Gets begin iterator of the container. The iterator should not be
   * dereferenced in case if the container is empty.
   */
  Iterator Begin() {
    Node* node = root_;
    if (!node) return {};
    while (node->left) {
      node = node->left;
    }
    return Iterator{node};
  }
  /**
   * @brief Gets past the end iterator of the container. The iterator should
   * never be dereferenced.
   */
  Iterator End() { return {}; }

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

}  // namespace alpa

#endif  // INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H
