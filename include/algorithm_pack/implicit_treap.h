#ifndef INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H
#define INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H

#include <cassert>
#include <random>
#include <utility>

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
  void Insert(const T& value, size_t pos);
  /**
   * @brief Deletes the element from the container, which is stored in the given
   * position.
   * TODO What if the given position is out of range?
   */
  void Erase(size_t pos);
  /**
   * @brief Performs cyclic rotation of the container on the given position
   * number to the right. If the given number is less than 0, the cyclic
   * rotation to the left will be performed.
   */
  void Rotate(int count);
  /**
   * @brief Gets begin iterator of the container. The iterator should not be
   * dereferenced in case if the container is empty.
   * TODO Are iterators invalidated after insrtion/delition?
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
   * TODO Study interator invalidation
   */
  Iterator End() { return {}; }

 private:
  struct Node {
    Node(const T& val, uint64_t p) : priority(p), value(val) {}

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    size_t tree_size = 1;  // includes current node
    uint64_t priority = 0;
    T value;
  };
  static size_t GetTreeSize(Node* node) { return node ? node->tree_size : 0; }
  /**@brief Sets size of the given node according to its children*/
  static void FixTreeSize(Node* node) {
    node->tree_size = GetTreeSize(node->left) + GetTreeSize(node->right) + 1;
  }
  static void FixParent(Node* node) {
    if (!node) return;
    if (node->left) node->left->parent = node;
    if (node->right) node->right->parent = node;
  }
  /**
   * @brief Merges two trees passed via their roots.
   * Requires that all keys in the left tree ARE NOT LARGER than keys in the
   * right tree.
   * Can return nullptr if both input parameters are nullptr.
   */
  static Node* Merge(Node* lhs, Node* rhs) {
    if (!lhs) return rhs;
    if (!rhs) return lhs;
    Node* root = nullptr;
    if (lhs->priority > rhs->priority) {
      // lhs root should be on top
      root = lhs;
      root->left = lhs->left;
      root->right = Merge(lhs->right, rhs);
    } else {
      // rhs root should be on top
      root = rhs;
      root->right = rhs->right;
      root->left = Merge(lhs, rhs->left);
    }
    FixParent(root);
    FixTreeSize(root);
  }
  /**
   * @brief Splits current tree in two. The first tree in the result contains
   * all keys which are smaller than key, the second - all the other.
   * If one of the resulting trees is empty, nullptr is returned.
   */
  static std::pair<Node*, Node*> Split(size_t pos, Node* node) {
    std::pair<Node*, Node*> result{nullptr, nullptr};
    if (!node) return result;
    if (node->tree_size < pos) {
      // node and its left child should be stored in the first field
      result.first = node;
      result.first->left = node->left;
      auto [smaller, other] = Split(pos, node->right);
      result.first->right = smaller;
      result.second = other;
    } else {
      // node and its right child should be stored in the right field
      result.second = node;
      result.second->right = node->right;
      auto [smaller, other] = Split(pos, node->right);
      result.second->left = other;
      result.first = smaller;
    }
    FixParent(result.first);
    FixTreeSize(result.first);
    FixParent(result.second);
    FixTreeSize(result.second);
    return result;
  }

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
