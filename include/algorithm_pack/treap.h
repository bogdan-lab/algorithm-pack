#ifndef INCLUDE_ALGORITHM_PACK_TREAP_H
#define INCLUDE_ALGORITHM_PACK_TREAP_H

#include <cassert>
#include <random>
#include <utility>

namespace alpa {
/**
 * @brief Realization of the treap (tree + heap) structure as template.
 *
 * Class represents a binary tree, which uses as a key a combination of the user
 * given key and priority. The resulted tree behaves like binary search tree
 * according to the user provided keys and behaves like a heap according to
 * priorities. If the priority of all keys are random the tree will be balanced.
 * The tree stores only unique user given keys. For the key type one has to
 * determine `operator<()`
 */
template <typename K, typename V>
class Treap {
 public:
  /**
   * @brief Constructs an empty tree with default seed.
   *
   * The seed is used in random generator for providing priorities.
   */
  Treap() = default;
  /**
   * @brief  Constructs empty tree initialized with the given seed.
   *
   * The seed is used in random generator for providing priorities.
   */
  Treap(uint64_t seed) : rnd_(seed) {}
  /**
   * @brief Destroy the Treap object by deleting each node.
   */
  ~Treap() { DeleteTree(root_); }
  /**
   * @brief Sets seed of the random generator associated with current tree.
   *
   * Random generator is used for creating priorities.
   */
  void SetSeed(uint64_t seed) { rnd_.seed(seed); }
  /**
   * @brief Inserts given (key, value) into the tree.
   *
   * If the given key is already present in the tree, the later is unchanged.
   * Returns pointer to the inserted value. Complexity O(log n)
   *
   * @param key user provided key with which the value should be stored
   * @param value value which is intended to be stored
   *
   * @return V* pointer to the inserted value. If the insertion was successful
   * pointer to the new value will be returned. If such key is already present
   * in the treap the pointer to its value will be returned. Cannot return
   * nullptr.
   */
  V* Insert(const K& key, const V& value) {
    V* old_value = Find(key);
    if (old_value) return old_value;
    uint64_t priority = rnd_();
    Node* parent = nullptr;
    Node* curr_ptr = root_;
    while (curr_ptr) {
      if (curr_ptr->priority <= priority) break;
      parent = curr_ptr;
      curr_ptr = key < curr_ptr->item.first ? curr_ptr->left : curr_ptr->right;
    }
    Node* new_node = new Node(key, value, priority);
    if (curr_ptr) {
      std::pair<Node*, Node*> splitted = Split(key, curr_ptr);
      new_node->left = splitted.first;
      new_node->right = splitted.second;
    }
    if (!parent) {
      root_ = new_node;
    } else {
      AddChildToParent(parent, new_node, /*to_left=*/key < parent->item.first);
    }
    ++size_;
    return &new_node->item.second;
  }
  /**
   * @brief Removes the key and associated value from the treap.
   *
   * If there is no key equivalent to the given one the treap will not be
   * changed. Complexity O(log n).
   *
   * @param key the key which is inteded to be removed from the treap
   *
   * @return true if the equivalent key was found in the treap and it was
   * deleted, false otherwise.
   */
  bool Erase(const K& key) {
    Node* parent = nullptr;
    Node* curr_ptr = root_;
    while (curr_ptr) {
      if (curr_ptr->item.first < key) {
        parent = curr_ptr;
        curr_ptr = curr_ptr->right;
      } else if (key < curr_ptr->item.first) {
        parent = curr_ptr;
        curr_ptr = curr_ptr->left;
      } else {
        break;
      }
    }
    if (!curr_ptr) return false;
    Node* replacement = Merge(curr_ptr->left, curr_ptr->right);
    if (!parent) {
      root_ = replacement;
    } else {
      AddChildToParent(parent, replacement,
                       /*to_left=*/key < parent->item.first);
    }
    delete curr_ptr;
    --size_;
    if (!size_) root_ = nullptr;
    return true;
  }
  /**
   * @brief Searches the given key in the treap.
   *
   * @param key key to be find in the treap.
   *
   * @return V* non owning pointer to the value associated with the given key.
   * If the key is not found nullptr will be returned.
   */
  V* Find(const K& key) {
    Node* curr_ptr = root_;
    while (curr_ptr) {
      if (key < curr_ptr->item.first) {
        curr_ptr = curr_ptr->left;
      } else if (curr_ptr->item.first < key) {
        curr_ptr = curr_ptr->right;
      } else {
        return &curr_ptr->item.second;
      }
    }
    return nullptr;
  }
  /**
   * @brief Checks whether the treap is empty or not.
   * @return true if the treap is empty, false otherwise.
   */
  bool Empty() const { return root_ == nullptr; }
  /**
   * @brief Gets the number of elements in the treap.
   */
  size_t Size() const { return size_; }

 private:
  /**
   * @brief Describes single node in the treap.
   */
  struct Node {
    /**
     * @brief Construct a new Node object with given parameters
     *
     * @param k node key
     * @param v node value
     * @param p node priority
     */
    Node(const K& k, const V& v, uint64_t p) : item(k, v), priority(p) {}

    std::pair<const K, V> item;
    uint64_t priority = 0;
    Node* left = nullptr;
    Node* right = nullptr;
  };
  /**
   * @brief Merges two treaps passed as their roots.
   *
   * Note that method does not perform any copying, it simply rearranges
   * pointers. Complexity O(log n).
   *
   * @param lhs the first treap root
   * @param rhs the second treap root
   * @warning Requires that all keys in the left tree are not larger than keys
   * in the right tree.
   * @return root of the merged treap. Can return nullptr if both input
   * parameters are nullptr.
   */
  static Node* Merge(Node* lhs, Node* rhs) {
    if (!lhs) return rhs;
    if (!rhs) return lhs;
    Node* root = nullptr;
    if (lhs->priority > rhs->priority) {
      // lhs root has to be on top
      root = lhs;
      root->left = lhs->left;
      root->right = Merge(lhs->right, rhs);
    } else {
      // rhs root has to be on top
      root = rhs;
      root->left = Merge(lhs, rhs->left);
      root->right = rhs->right;
    }
    return root;
  }
  /**
   * @brief Splits current tree in two.
   *
   * Method does not perform any copying, it simply rearranges pointers.
   * Complexity O(log n).
   *
   * @param key key according to which split will be performed. Does not
   * required to be in the treap.
   * @param root root of the treap which is inteded to be splitted. Can be
   * nullptr.
   *
   * @return two treap roots. The first treap in the result contains
   * all keys which are smaller than the given key, the second - contains all
   * the other. If one of the resulting trees is empty, the corresponding root
   * is nullptr.
   */
  static std::pair<Node*, Node*> Split(const K& key, Node* root) {
    std::pair<Node*, Node*> result{nullptr, nullptr};
    if (!root) return result;
    if (root->item.first < key) {
      // root and its left child goes to the first tree
      result.first = root;
      result.first->left = root->left;
      auto splitted_right = Split(key, root->right);
      result.first->right = splitted_right.first;
      result.second = splitted_right.second;
    } else {
      // root and its right child goes to the second tree
      result.second = root;
      result.second->right = root->right;
      auto splitted_left = Split(key, root->left);
      result.second->left = splitted_left.second;
      result.first = splitted_left.first;
    }
    return result;
  }
  /**
   * @brief Deletes all nodes in the treap with the given root. Complexity O(n).
   * @param root root of treap to be deleted. Can be nullptr.
   */
  static void DeleteTree(Node* root) {
    if (!root) return;
    DeleteTree(root->left);
    DeleteTree(root->right);
    delete root;
  }
  /**
   * @brief Adds child to the given parent. Parent pointer should not be
   * nullptr.
   * @param parent pointer to the parent node. Should not be nullptr.
   * @param child child node to be added to the parent. Can be nullptr.
   * @param to_left true if the child should be added a as the left child of the
   * parent, false if the child should be added as the right child of the
   * parent.
   */
  static void AddChildToParent(Node* parent, Node* child, bool to_left) {
    assert(parent);
    if (to_left) {
      parent->left = child;
    } else {
      parent->right = child;
    }
  }

  Node* root_ = nullptr;
  std::mt19937_64 rnd_;
  size_t size_ = 0;
};
}  // namespace alpa
#endif  // INCLUDE_ALGORITHM_PACK_TREAP_H
