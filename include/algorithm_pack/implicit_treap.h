#ifndef INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H
#define INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H

#include <cassert>
#include <random>
#include <utility>

namespace alpa {
/**
 * @brief Realization of a treap with an implicit key.
 *
 * Basically the structure is an node based array with improved asymptotic
 * complexity of some operations. In current realization, insertion, deletion
 * and rotation have O(log n) complexity, where n is container size.
 */
template <typename T>
class ImplicitTreap {
  struct Node;

 public:
  /**
   * @brief Represents constant bidirectional iterator for the ImplicitTreap
   * structure.
   */
  class ConstIterator {
   public:
    friend class ImplicitTreap;
    friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
      assert(lhs.host_ == rhs.host_);
      return lhs.curr_node_ == rhs.curr_node_;
    }
    friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) {
      assert(lhs.host_ == rhs.host_);
      return lhs.curr_node_ != rhs.curr_node_;
    }
    /**
     * @brief Creates empty iterator. It is invalid. Therefore result of
     * dereferencing or comparing operations is not specified.
     */
    ConstIterator() = default;
    /**
     * @brief Performs pre-increment operation. Should be called only on valid
     * iterators.
     *
     * @return ConstIterator& reference to the incremented iterator.
     */
    ConstIterator& operator++() {
      curr_node_ = ImplicitTreap::GetNextNode(curr_node_);
      return *this;
    }
    /**
     * @brief Performs post-increment operation. Should be called only on valid
     * iterators.
     *
     * @return ConstIterator the instance of the iterator before it was
     * incremented.
     */
    ConstIterator operator++(int) {
      const Node* old_node = curr_node_;
      curr_node_ = ImplicitTreap::GetNextNode(curr_node_);
      return ConstIterator{old_node, host_};
    }
    /**
     * @brief Performs pre-decrement operation. Should be called only on valid
     * iterators.
     *
     * @return ConstIterator& reference to the decremented iterator.
     */
    ConstIterator& operator--() {
      assert(host_);
      assert(host_->root_);
      curr_node_ = curr_node_ ? ImplicitTreap::GetPrevNode(curr_node_)
                              : ImplicitTreap::FindLastNode(host_->root_);
      return *this;
    }
    /**
     * @brief Performs post-decrement operation. Should be called only on valid
     * iterators.
     *
     * @return ConstIterator the instance of the iterator before it was
     * decremented.
     */
    ConstIterator operator--(int) {
      assert(host_);
      assert(host_->root_);
      const Node* old_node = curr_node_;
      curr_node_ = curr_node_ ? ImplicitTreap::GetPrevNode(curr_node_)
                              : ImplicitTreap::FindLastNode(host_->root_);
      return ConstIterator{old_node, host_};
    }
    /**
     * @brief Dereferences this iterator. Should be called only on valid
     * iterator.
     *
     * @return const T& reference to the value stored in the given position.
     */
    const T& operator*() const { return curr_node_->value; }
    /**
     * @brief Provides constant access to the value pointed by iterator.
     *
     * @return const T* pointer to the value at which iterator currently points.
     */
    const T* operator->() const { return &curr_node_->value; }

   private:
    /**
     * @brief Construct a new ConstIterator object for the given treap.
     *
     * @param node - will be used as current iterator node
     * @param host - pointer to the treap, so we can properly iterate backwards
     * starting from the end() iterator.
     */
    explicit ConstIterator(const Node* node, const ImplicitTreap* host)
        : curr_node_(node), host_(host) {}

    const Node* curr_node_ = nullptr;
    const ImplicitTreap* host_ = nullptr;
  };
  /**
   * @brief Represents bidirectional iterator for the ImplicitTreap structure.
   */
  class Iterator {
   public:
    friend class ImplicitTreap;
    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
      assert(lhs.host_ == rhs.host_);
      return lhs.curr_node_ == rhs.curr_node_;
    }
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
      assert(lhs.host_ == rhs.host_);
      return lhs.curr_node_ != rhs.curr_node_;
    }
    /**
     * @brief Creates empty iterator. It is invalid. Therefore result of
     * dereferencing or comparing operations is not specified.
     */
    Iterator() = default;
    /**
     * @brief Converts modifiable iterator to the constant by creating the
     * later.
     *
     * @return ConstIterator object created from this iterator.
     */
    operator ConstIterator() const { return ConstIterator{curr_node_, host_}; }
    /**
     * @brief Performs pre-increment operation. Should be called only on valid
     * iterators.
     *
     * @return Iterator& reference to the incremented iterator.
     */
    Iterator& operator++() {
      curr_node_ = ImplicitTreap::GetNextNode(curr_node_);
      return *this;
    }
    /**
     * @brief Performs post-increment operation. Should be called only on valid
     * iterators.
     *
     * @return Iterator the instance of the iterator before it was
     * incremented.
     */
    Iterator operator++(int) {
      Node* old_node = curr_node_;
      curr_node_ = ImplicitTreap::GetNextNode(curr_node_);
      return Iterator{old_node, host_};
    }
    /**
     * @brief Performs pre-decrement operation. Should be called only on valid
     * iterators.
     *
     * @return Iterator& reference to the decremented iterator.
     */
    Iterator& operator--() {
      assert(host_);
      assert(host_->root_);
      curr_node_ = curr_node_ ? ImplicitTreap::GetPrevNode(curr_node_)
                              : ImplicitTreap::FindLastNode(host_->root_);
      return *this;
    }
    /**
     * @brief Performs post-decrement operation. Should be called only on valid
     * iterators.
     *
     * @return Iterator the instance of the iterator before it was
     * decremented.
     */
    Iterator operator--(int) {
      assert(host_);
      assert(host_->root_);
      Node* old_node = curr_node_;
      curr_node_ = curr_node_ ? ImplicitTreap::GetPrevNode(curr_node_)
                              : ImplicitTreap::FindLastNode(host_->root_);
      return Iterator{old_node, host_};
    }
    /**
     * @brief Dereferences this iterator. Should be called only on valid
     * iterator.
     *
     * @return T& reference to the value stored in the given position.
     */
    T& operator*() { return curr_node_->value; }
    /**
     * @brief Provides access to the value pointed by iterator.
     *
     * @return T* pointer to the value at which iterator currently points.
     */
    T* operator->() const { return &curr_node_->value; }

   private:
    /**
     * @brief Construct a new Iterator object for the given treap.
     *
     * @param node - will be used as a current iterator node.
     * @param host - pointer to the treap, so we can properly iterate from the
     * backwards starting from the end()
     */
    explicit Iterator(Node* node, ImplicitTreap* host)
        : curr_node_(node), host_(host) {}

    Node* curr_node_ = nullptr;
    ImplicitTreap* host_ = nullptr;
  };

  /**
   * @brief Creates an empty treap.
   */
  ImplicitTreap() = default;
  /**
   * @brief Creates an empty treap with the given seed set in the random
   * generator
   */
  ImplicitTreap(uint64_t seed) : rnd_(seed) {}
  ImplicitTreap(const ImplicitTreap& other) = delete;
  ImplicitTreap& operator=(const ImplicitTreap& other) = delete;
  /**
   * @brief Construct a new Implicit Treap object by moving data from other.
   *
   * @param other - object from which data is moved from. All iterators of this
   * object become invalidated.
   */
  ImplicitTreap(ImplicitTreap&& other) {
    root_ = std::exchange(other.root_, nullptr);
    rnd_ = std::move(other.rnd_);
    size_ = std::exchange(other.size_, 0);
  }
  /**
   * @brief Replaces current treap data by the data from other. Old data is
   * destroyed.
   *
   * Does nothing if this treap and othe has the same root. Note that check for
   * the root equality is the only which is performed. In other words, operator
   * does not consider situations when two treaps share same nodes deeper in the
   * structure.
   *
   * @param other - object from which data is moved from. If moving from this
   * object was performed, all its iterators become invalidated.
   * @return ImplicitTreap& reference to the new treap, which contains data from
   * other.
   */
  ImplicitTreap& operator=(ImplicitTreap&& other) {
    if (other.root_ == root_) return *this;
    DeleteTree(root_);
    root_ = std::exchange(other.root_, nullptr);
    rnd_ = std::move(other.rnd_);
    size_ = std::exchange(other.size_, 0);
    return *this;
  }
  /**
   * @brief Constructs a new Implicit Treap object, which will contain all
   * elements from the given vector. Complexity O(n).
   *
   * @param input element collection which will be copied to the treap. Can be
   * empty.
   * @param seed will set in radom generator which generates priorities.
   */
  ImplicitTreap(const std::vector<T>& input, uint64_t seed) : rnd_(seed) {
    if (input.empty()) return;
    size_ = input.size();
    auto it = input.begin();
    root_ = new Node(*it++, /*priority=*/rnd_());
    Node* last_included = root_;
    while (it != input.end()) {
      Node* new_node = new Node(*it++, /*priority=*/rnd_());
      while (last_included && last_included->priority < new_node->priority) {
        last_included = last_included->parent;
      }
      if (!last_included) {
        // New node has the largest priority
        new_node->left = root_;
        new_node->left->parent = new_node;
        FixTreeSize(new_node);
        root_ = new_node;
      } else {
        // Here last_include->priority >= new_node->priority
        if (last_included->right) {
          new_node->left = last_included->right;
          new_node->left->parent = new_node;
          FixTreeSize(new_node);
        }
        last_included->right = new_node;
        last_included->right->parent = last_included;
        while (last_included) {
          FixTreeSize(last_included);
          last_included = last_included->parent;
        }
      }
      last_included = new_node;
    }
  }
  /**
   * @brief Destroy the Implicit Treap object by destroying each value it
   * stored.
   */
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
   * @brief Inserts the given value into given position by copying it.
   * Complexity O(log n). Does not ivalidate iterators.
   *
   * @param value - actual value which needs to be placed into the treap.
   * @param pos - position where the new element should be inserted. If the
   * given position is larger than the container size, new element will be
   * stored as the new last element. Position numeration starts from 0.
   * @return T& reference to the value stored in the container.
   */
  T& Insert(const T& value, size_t pos) {
    ++size_;
    Node* new_node = new Node(value, /*priority=*/rnd_());
    auto [left, right] =
        Split(/*element_number=*/std::min(size_, pos) + 1, root_);
    root_ = Merge(Merge(left, new_node), right);
    return new_node->value;
  }
  /**
   * @brief Concatenates the given treap to the end of the current one.
   * Complexity O(log n). Does not invalidate iterators.
   *
   * @param other given treap which will be concatenated. Ownership of all
   * elements from the given treap will be moved to this treap.
   * @return ImplicitTreap<T>& reference to the concatenated treap
   */
  ImplicitTreap<T>& Concatenate(ImplicitTreap&& other) {
    root_ = Merge(root_, std::exchange(other.root_, nullptr));
    size_ += std::exchange(other.size_, 0);
    return *this;
  }
  /**
   * @brief Returns reference to the element stored in the given position.
   * Complexity O(log n)
   *
   * @param pos - position of the requested element. Given position should be
   * valid, this is it should be in the range [0, Size()).
   * @return T& reference to the stored element
   */
  T& operator[](size_t pos) {
    assert(root_);
    assert(pos < size_);
    return GetElement(root_, pos + 1)->value;
  }
  /**
   * @overload
   */
  const T& operator[](size_t pos) const {
    assert(root_);
    assert(pos < size_);
    return GetElement(root_, pos + 1)->value;
  }
  /**
   * @brief Deletes the element from the container, which is stored in the given
   * position. Complexity O(log n). Invalidates only iterators which pointed to
   * the deleted object.
   *
   * @param pos - position of the element to be deleted. Method expects that
   * given position is valid, this is its value is in range [0, treap_size).
   */
  void Erase(size_t pos) {
    assert(root_);
    std::pair<Node*, Node*> first_split = Split(pos + 1, root_);
    std::pair<Node*, Node*> second_split = Split(2, first_split.second);
    delete second_split.first;
    root_ = Merge(first_split.first, second_split.second);
    --size_;
  }
  /**
   * @brief Performs cyclic rotation of the container on the given position
   * number to the right.
   *
   * count sign defins the direction of the rotaiton:
   * count = 1 results in [1, 2, 3, 4] -> [4, 1, 2, 3]
   * count = -1 results in [1, 2, 3, 4] -> [2, 3, 4, 1]
   * count = 0 - does nothing
   *
   * Method does not invalidate any iterators.
   * Method should not be called on the empty treap
   * @param count - number of positions we need to rotate the container. We can
   * have absolute value larger than treap size. Complexity O(log n).
   */
  void Rotate(int count) {
    if (count == 0) {
      return;
    } else if (count > 0) {
      count = count % size_;
      if (!count) return;
      std::pair<Node*, Node*> split = Split(size_ - count + 1, root_);
      root_ = Merge(split.second, split.first);
    } else {
      count = -count % size_;
      if (!count) return;
      std::pair<Node*, Node*> split = Split(count + 1, root_);
      root_ = Merge(split.second, split.first);
    }
  }
  /**
   * @brief Gets begin iterator of the container. Complexity O(log n).
   */
  Iterator Begin() { return Iterator{FindFirstNode(root_), this}; }
  /**
   * @overload
   */
  ConstIterator Begin() const {
    return ConstIterator{FindFirstNode(root_), this};
  }
  /**
   * @brief Gets ContantIterator to the container. Complexity O(log n);
   *
   * @return ConstIterator
   */
  ConstIterator CBegin() const {
    return ConstIterator{FindFirstNode(root_), this};
  }
  /**
   * @brief Gets past the end iterator of the container. Complexity constant.
   */
  Iterator End() { return Iterator{nullptr, this}; }
  /**
   * @overload
   */
  ConstIterator End() const { return ConstIterator{nullptr, this}; }
  /**
   * @brief Gets ConstantIterator to the container. Complexity constant.
   *
   * @return ConstIterator
   */
  ConstIterator CEnd() const { return ConstIterator{nullptr, this}; }
  /**
   * @brief Converts the structure into vector by copying each element into one.
   * Complexity O(n).
   * @return Created vector, which containes all elements from the treap.
   */
  std::vector<T> ConvertToVector() const {
    std::vector<T> result;
    result.reserve(Size());
    for (auto it = CBegin(); it != CEnd(); ++it) {
      result.push_back(*it);
    }
    return result;
  }

 private:
  /**
   * @brief Describes single element stored in the treap.
   */
  struct Node {
    Node(const T& val, uint64_t p) : priority(p), value(val) {}
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    /**Number of elements in this node subtree, including itself.*/
    size_t tree_size = 1;
    uint64_t priority = 0;
    T value;
  };
  /**
   * @brief Calculates the tree size of the subtree which corresponds to the
   * given node.
   *
   * @param node - root of the tree which is processed. Can be nullptr.
   * @return size_t number of elements in the given tree.
   */
  static size_t GetTreeSize(Node* node) { return node ? node->tree_size : 0; }
  /**
   * @brief Sets size of the given node according to its children
   * @param node - node needed to be fixed. Cannot be nullptr.
   */
  static void FixTreeSize(Node* node) {
    node->tree_size = GetTreeSize(node->left) + GetTreeSize(node->right) + 1;
  }
  /**
   * @brief Sets parent field of the given node childrens
   *
   * @param node - node which is needed to be fixed. Cannot be nullptr.
   */
  static void FixParent(Node* node) {
    if (node->left) node->left->parent = node;
    if (node->right) node->right->parent = node;
  }
  /**
   * @brief Merges two trees passed via their roots.
   *
   * Merge will be performed in the manner, that all elements which was in the
   * left tree will precede all elements in the right tree in case of container
   * traversion.  No elements are copied. Method only rearranges pointers.
   * Complexity O(log n)
   *
   * @param lhs - root of the left treap
   * @param rhs - root of the right treap
   * @return Node* root of the merged treap. Can return nullptr, if both input
   * treaps are empty.
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
    return root;
  }
  /**
   * @brief Splits current tree in two according to the given element number.
   *
   * All elements, which number is smaller than given one will be in the
   * first tree and all other elements - in the second.  Complexity O(log n).
   *
   * @param el_number - element number according to which treap is being
   * splitted. Note that element number, unlike element index, starts from 1.
   * @param node - root of the treap, which is being splitted.
   * @return Roots of two treaps in which the original one was splitted. This
   * is, parent of each root node is nullptr. The resulting pair can contain
   * nullptr.
   */
  static std::pair<Node*, Node*> Split(size_t el_number, Node* node) {
    std::pair<Node*, Node*> result{nullptr, nullptr};
    if (!node) return result;
    size_t elements_until_this = GetTreeSize(node->left) + 1;
    if (elements_until_this < el_number) {
      // node and its left child should be stored in the first field
      result.first = node;
      result.first->left = node->left;
      auto [smaller, other] =
          Split(el_number - elements_until_this, node->right);
      result.first->right = smaller;
      result.second = other;
    } else {
      // node and its right child should be stored in the right field
      result.second = node;
      result.second->right = node->right;
      auto [smaller, other] = Split(el_number, node->left);
      result.second->left = other;
      result.first = smaller;
    }
    if (result.first) {
      FixParent(result.first);
      FixTreeSize(result.first);
      result.first->parent = nullptr;
    }
    if (result.second) {
      FixParent(result.second);
      FixTreeSize(result.second);
      result.second->parent = nullptr;
    }

    return result;
  }
  /**
   * @brief Recursively destroys all elements in the treap.
   *
   * @param root - root of the given treap
   */
  static void DeleteTree(Node* root) {
    if (!root) return;
    DeleteTree(root->left);
    DeleteTree(root->right);
    delete root;
  }
  /**
   * @brief Gets the node which is the next after the given one in the treap
   * traversing order. Complexity O(log n), in average amortized constant.
   *
   * @param curr_node - node relatively to which we are searching the next one.
   * Cannot be nullptr.
   * @return Node* pointer to the next node. Can be nullptr.
   */
  static Node* GetNextNode(const Node* curr_node) {
    assert(curr_node);
    Node* right = curr_node->right;
    if (!right) {
      // Search parent from which we went left
      const Node* node = curr_node;
      Node* parent = curr_node->parent;
      while (parent) {
        if (parent->left == node) break;
        node = parent;
        parent = node->parent;
      }
      return parent;
    }
    while (right->left) {
      right = right->left;
    }
    return right;
  }
  /**
   * @brief Gets the node which is the previous relatively to the given one in
   * the treap traversing order. Complexity O(log n), in average amortized
   * constant.
   *
   * @param curr_node - node relatively to which we are searching the previous
   * one. Cannot be nullptr.
   * @return Node* pointer to the next node. Can be nullptr.
   */
  static Node* GetPrevNode(const Node* curr_node) {
    assert(curr_node);
    Node* left = curr_node->left;
    if (!left) {
      // Search parent from which we went right
      const Node* node = curr_node;
      Node* parent = curr_node->parent;
      while (parent) {
        if (parent->right == node) break;
        node = parent;
        parent = node->parent;
      }
      return parent;
    }
    while (left->right) {
      left = left->right;
    }
    return left;
  }
  /**
   * @brief Returns the first element in the given treap.
   *
   * @param root - root of the given treap. Can be nullptr.
   * @return Node* pointer to the first node in the treap. Can return nullptr.
   */
  static Node* FindFirstNode(Node* root) {
    if (!root) return root;
    while (root->left) {
      root = root->left;
    }
    return root;
  }
  /**
   * @brief Returns the last node in the given treap.
   *
   * @param root - root of the given treap. Has to be valid pointer.
   * @return Node* pointer to the last node. Cannot be nullptr.
   */
  static Node* FindLastNode(Node* root) {
    assert(root);
    while (root->right) {
      root = root->right;
    }
    return root;
  }
  /**
   * @brief Gets the node with the given element number.
   *
   * Method expects that the element with the requested element number is
   * available in the treap.
   *
   * @param root - root of the treap, where we are looking for the element.
   * @param el_number - element number. Unlike element index, element number
   * starts from 1.
   * @return Node* pointer to the requested element. Never returns nullptr.
   */
  static Node* GetElement(Node* root, size_t el_number) {
    assert(el_number > 0);
    assert(root);
    int curr_el_number = GetTreeSize(root->left) + 1;
    if (el_number < curr_el_number) {
      return GetElement(root->left, el_number);
    } else if (el_number > curr_el_number) {
      return GetElement(root->right, el_number - curr_el_number);
    } else {
      return root;
    }
  }

  Node* root_ = nullptr;
  std::mt19937_64 rnd_;
  size_t size_ = 0;
};

}  // namespace alpa

#endif  // INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H
