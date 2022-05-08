#ifndef INCLUDE_ALGORITHM_PACK_TREAP_H
#define INCLUDE_ALGORITHM_PACK_TREAP_H

#include <utility>

/**
 * @brief Realization of the treap (tree + heap) structure as template.
 * Class has node structure.
 *
 * TODO Will this be balanced tree by randomization of the priority parameter or
 * we want to allow the user to set priority parameter themselves? Why doe one
 * will need it?
 */
template <typename K, typename V>
class Treap {
 public:
  /**
   * @brief TODO Does method create any copies?
   * double delete problem or we should move structures here?
   */
  static Treap Merge(const Treap& lhs, const Treap& rhs);

  /**
   * @brief Splits current tree in two two according to the given key.
   * TODO Will I perform copying here?
   */
  static std::pair<Treap, Treap> Split(const K& key, const Treap& input);

  /**
   * @brief TODO Do we guarantee that we will store only unique copies???
   */
  bool Insert(const K& key, const V& value);

  /**
   * @brief TODO What will be here if we store more than one element with given
   * key?
   */
  bool Erase(const K& key);

  /**
   * TODO Should return here node or something like it, otherwise it looks like
   * I ignore values.
   */
  bool Find(const K& key) const;

  /**@brief Returns true if treap is empty, false otherwise.*/
  bool Empty() const { return root_ == nullptr; }

 private:
  struct Node {
    std::pair<K, V> item;
    Node* left = nullptr;
    Node* right = nullptr;
  };

  Node* root_ = nullptr;
}

#endif  // INCLUDE_ALGORITHM_PACK_TREAP_H
