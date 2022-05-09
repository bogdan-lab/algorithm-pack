#ifndef INCLUDE_ALGORITHM_PACK_TREAP_H
#define INCLUDE_ALGORITHM_PACK_TREAP_H

#include <ctime>
#include <random>
#include <utility>

/**
 * @brief Realization of the treap (tree + heap) structure as template.
 * Class represents a binary tree, which uses as key, user given key combined
 * with random priority. The resulting tree structure is balanced. Current
 * structure stores only unique user given keys.
 * For the key type one has to determine `operator<()`
 */
template <typename K, typename V>
class Treap {
 public:
  /**
   * @brief Inserts given (key, value) into the tree. If the given key is
   * laready present in the tree, the later is unchanged. Returns pointer to the
   * inserted value. This is, if the insertion was successfull method will
   * return the pointer to the newly inserted value, otherwise it will return
   * the pointer to the old value, found in the tree. Method will never return
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
    AddChildToParent(parent, new_node);
    return &new_node->item.second;
  }

  /**
   * @brief Removes node with the given key from the tree. If there is no node
   * with such key in the tree, nothing will happen. Retutns true if the node
   * was deleted, false otherwise.
   */
  bool Erase(const K& key) {
    if (!root_) return false;
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
    AddChildToParent(parent, Merge(curr_ptr->left, curr_ptr->right));
    return true;
  }

  /**
   * @brief Returns non owning pointer to the corresponding value. If the given
   * key is not present in the tree, nullptr is returned.
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

  /**@brief Returns true if treap is empty, false otherwise.*/
  bool Empty() const { return root_ == nullptr; }

 private:
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
   * @brief Splits current tree in two. The first tree in the result contains
   * all keys which are smaller than key, the second - all the other.
   * If one of the resulting trees is empty, nullptr is returned.
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
      result.second->first = splitted_left.second;
      result.first = splitted_left.first;
    }
    return result;
  }
  /**
   * @brief Adds child to the given parent. If the parent is nullptr, the child
   * is treated as new root and stored into the corresponding field.
   */
  void AddChildToParent(Node* parent, Node* child) {
    if (!parent) {
      root_ = child;
      return;
    }
    if (parent->item.first < child->item.first) {
      parent->right = child;
    } else {
      parent->left = child;
    }
  }

  struct Node {
    Node(const K& k, const V& v, uint64_t p) : item(k, v), priority(p) {}

    std::pair<const K, V> item;
    uint64_t priority = 0;
    Node* left = nullptr;
    Node* right = nullptr;
  };

  static std::mt19937_64 rnd_{std::time(nullptr)};

  Node* root_ = nullptr;
}

#endif  // INCLUDE_ALGORITHM_PACK_TREAP_H
