#ifndef INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H
#define INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H

#include <cassert>
#include <iterator>
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
   * @brief Represents constant random access iterator for the ImplicitTreap
   * structure.
   */
  class ConstIterator {
   public:
    friend class ImplicitTreap;

    using iterator_category = std::random_access_iterator_tag;
    using difference_type = int;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;

    friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
      assert(lhs.host_ == rhs.host_);
      return lhs.curr_node_ == rhs.curr_node_;
    }
    friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) {
      assert(lhs.host_ == rhs.host_);
      return lhs.curr_node_ != rhs.curr_node_;
    }
    /**
     * @brief Shifts iterator in the random access manner to the right.
     * Complexity O(log n).
     *
     * @param lhs starting iterator from which shift is performed.
     * @param shift number of position on which iterator has to be shifted. If
     * negative iterator will be shifted to the left. The shift value should be
     * correct, this is after shifting iterator should remain in the range
     * [begin, end).
     * @return ConstIterator new shifted iterator.
     */
    friend ConstIterator operator+(const ConstIterator& lhs,
                                   difference_type shift);
    /**
     * @overload
     */
    friend ConstIterator operator+(difference_type shift,
                                   const ConstIterator& rhs) {
      return rhs + shift;
    }
    /**
     * @brief Shifts iterator in the random access manner to the left.
     * Complexity O(log n).
     *
     * @param lhs starting iterator from which shift is performed.
     * @param shift number of position on which iterator has to be shifted. If
     * negative iterator will be shifted to the right. The shift value should be
     * correct, this is after shifting iterator should remain in the range
     * [begin, end).
     * @return ConstIterator new shifted iterator.
     */
    friend ConstIterator operator-(const ConstIterator& lhs,
                                   difference_type shift) {
      return lhs + (-shift);
    }
    /**
     * @brief Returns the number of elelements between two iterators
     *
     * @param rhs the second iterator
     * @return int number of elements between two iterators, can be negative if
     * the rhs iterator preceedes this iterator.
     */
    friend difference_type operator-(const ConstIterator& lhs,
                                     const ConstIterator& rhs);
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
     * @brief Shifts current iterator to the given number of positions to the
     * right
     *
     * @param shift number of positions on which iterator should be shifted
     * @return ConstIterator& shifted iterator.
     */
    ConstIterator& operator+=(difference_type shift) {
      *this = *this + shift;
      return *this;
    }
    /**
     * @brief Shifts current iterator to the given number of positions to the
     * left
     *
     * @param shift number of positions on which iterator should be shifted
     * @return ConstIterator& shifted iterator.
     */
    ConstIterator& operator-=(difference_type shift) {
      *this = *this - shift;
      return *this;
    }
    /**
     * @brief Dereferences this iterator. Should be called only on valid
     * iterator.
     *
     * @return reference reference to the value stored in the given position.
     */
    reference operator*() const { return curr_node_->value; }
    /**
     * @brief Provides constant access to the value pointed by iterator.
     *
     * @return pointer pointer to the value at which iterator currently points.
     */
    pointer operator->() const { return &curr_node_->value; }

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
   * @brief Represents random access iterator for the ImplicitTreap structure.
   */
  class Iterator {
   public:
    friend class ImplicitTreap;

    using iterator_category = std::random_access_iterator_tag;
    using difference_type = int;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
      assert(lhs.host_ == rhs.host_);
      return lhs.curr_node_ == rhs.curr_node_;
    }
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
      assert(lhs.host_ == rhs.host_);
      return lhs.curr_node_ != rhs.curr_node_;
    }
    /**
     * @brief Shifts iterator in the random access manner to the right.
     * Complexity O(log n).
     *
     * @param shift number of position on which iterator has to be shifted. If
     * negative iterator will be shifted to the left. The shift value should be
     * correct, this is after shifting iterator should remain in the range
     * [begin, end).
     * @return Iterator new shifted iterator.
     */
    friend Iterator operator+(const Iterator& lhs, difference_type shift);
    /**
     * @overload
     */
    friend Iterator operator+(difference_type shift, const Iterator& rhs) {
      return rhs + shift;
    }
    /**
     * @brief Returns the number of elelements between two iterators
     *
     * @param rhs the second iterator
     * @return int number of elements between two iterators, can be negative if
     * the rhs iterator preceedes this iterator.
     */
    friend difference_type operator-(const Iterator& lhs, const Iterator& rhs);
    /**
     * @brief Shifts iterator in the random access manner to the left.
     * Complexity O(log n).
     *
     * @param lhs starting iterator from which shift is performed.
     * @param shift number of position on which iterator has to be shifted. If
     * negative iterator will be shifted to the right. The shift value should be
     * correct, this is after shifting iterator should remain in the range
     * [begin, end).
     * @return Iterator new shifted iterator.
     */
    friend Iterator operator-(const Iterator& lhs, difference_type shift) {
      return lhs + (-shift);
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
     * @brief Shifts current iterator to the given number of positions to the
     * right
     *
     * @param shift number of positions on which iterator should be shifted
     * @return ConstIterator& shifted iterator.
     */
    Iterator& operator+=(difference_type shift) {
      *this = *this + shift;
      return *this;
    }
    /**
     * @brief Shifts current iterator to the given number of positions to the
     * left
     *
     * @param shift number of positions on which iterator should be shifted
     * @return ConstIterator& shifted iterator.
     */
    Iterator& operator-=(difference_type shift) {
      *this = *this - shift;
      return *this;
    }
    /**
     * @brief Dereferences this iterator. Should be called only on valid
     * iterator.
     *
     * @return reference reference to the value stored in the given position.
     */
    reference operator*() { return curr_node_->value; }
    /**
     * @brief Provides access to the value pointed by iterator.
     *
     * @return pointer pointer to the value at which iterator currently points.
     */
    pointer operator->() const { return &curr_node_->value; }

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
   * Complexity O(1).
   *
   * @param other - object from which data is moved from. After this constructor
   * the state of the `other` is not valid.
   */
  ImplicitTreap(ImplicitTreap&& other) noexcept
      : root_(other.root_), rnd_(other.rnd_), size_(other.size_) {
    other.root_ = nullptr;
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
  ImplicitTreap& operator=(ImplicitTreap&& other) noexcept {
    ImplicitTreap tmp(std::move(other));
    Swap(tmp);
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
   * @brief Swaps the content of the other and current treaps. Complexity O(1).
   *
   * @param other its content will be replaced with the content of this treap.
   * All iterators are invalidated.
   */
  void Swap(ImplicitTreap& other) noexcept {
    std::swap(root_, other.root_);
    std::swap(rnd_, other.rnd_);
    std::swap(size_, other.size_);
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
   * @brief Extracts from the treap elements in the interval [start_pos,
   * end_pos).
   *
   * Element position numerationt starts from 0. The given range should
   * be valid, this is end_pos >= start_pos and end_pos <= treap size.
   * Complexity O(log n). Does not invalidate iterators.
   *
   * @param start_pos index of the first element which will be extracted.
   * @param end_pos index pass the last element in the extracting range.
   * @return ImplicitTreap treap which contains all extracted elements in the
   * preservedd order.
   */
  ImplicitTreap Extract(size_t start_pos, size_t end_pos) {
    assert(end_pos >= start_pos);
    assert(end_pos <= size_);
    ImplicitTreap result(/*seed=*/rnd_());
    if (start_pos == 0 && end_pos == size_) {
      result.root_ = std::exchange(root_, nullptr);
      result.size_ = std::exchange(size_, 0);
    } else if (start_pos < end_pos) {
      std::pair<Node*, Node*> start_split = Split(start_pos + 1, root_);
      size_t extracted_num = end_pos - start_pos;
      std::pair<Node*, Node*> end_split =
          Split(extracted_num + 1, start_split.second);
      result.root_ = end_split.first;
      result.size_ = extracted_num;
      root_ = Merge(start_split.first, end_split.second);
      size_ -= extracted_num;
    }
    return result;
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

 private:
  friend int operator-(const ImplicitTreap<T>::Iterator& lhs,
                       const ImplicitTreap<T>::Iterator& rhs) {
    assert(lhs.host_ == rhs.host_);
    size_t lhs_number = lhs.curr_node_ ? GetElementNumber(lhs.curr_node_)
                                       : lhs.host_->Size() + 1;
    size_t rhs_number = rhs.curr_node_ ? GetElementNumber(rhs.curr_node_)
                                       : rhs.host_->Size() + 1;
    return lhs_number - rhs_number;
  }
  friend int operator-(const ImplicitTreap<T>::ConstIterator& lhs,
                       const ImplicitTreap<T>::ConstIterator& rhs) {
    assert(lhs.host_ == rhs.host_);
    size_t lhs_number = lhs.curr_node_ ? GetElementNumber(lhs.curr_node_)
                                       : lhs.host_->Size() + 1;
    size_t rhs_number = rhs.curr_node_ ? GetElementNumber(rhs.curr_node_)
                                       : rhs.host_->Size() + 1;
    return lhs_number - rhs_number;
  }
  friend ConstIterator operator+(const ConstIterator& lhs, int shift) {
    assert(lhs.host_);
    if (!lhs.curr_node_) {
      // We are trying to shift from end() iterator
      return ConstIterator(
          GetElement(lhs.host_->root_, lhs.host_->size_ + shift + 1),
          lhs.host_);
    }
    return ConstIterator(ShiftNode(lhs.curr_node_, lhs.host_->root_, shift),
                         lhs.host_);
  }
  friend Iterator operator+(const Iterator& lhs, int shift) {
    assert(lhs.host_);
    if (!lhs.curr_node_) {
      // We are trying to shift from end() iterator
      return Iterator(
          GetElement(lhs.host_->root_, lhs.host_->size_ + shift + 1),
          lhs.host_);
    }
    return Iterator(ShiftNode(lhs.curr_node_, lhs.host_->root_, shift),
                    lhs.host_);
  }
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
  static size_t GetTreeSize(const Node* node) {
    return node ? node->tree_size : 0;
  }
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
  static void DeleteTree(Node* root) noexcept {
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
  /**
   * @brief Shifts current node to another valid node in the tree.
   * If shift value is not valid, this is, it results in shifting out of the
   * array bounds, nullptr will be returned.
   *
   * @param curr_node current node from which shift is calculated
   * @param root root of the tree inside which the shift is performed
   * @param shift number of elements on which we need to shift. Can be negative.
   * @return Node* shifted node or nullptr in case of incorrect input.
   */
  static Node* ShiftNode(const Node* curr_node, const Node* root, int shift) {
    size_t curr_number = GetElementNumber(curr_node);
    if (curr_number + shift < 1 || curr_number + shift > root->tree_size) {
      return nullptr;
    }
    return GetElement(const_cast<Node*>(root), curr_number + shift);
  }
  /**
   * @brief Calculates the element number which corresponds to the given node.
   * Complexity O(log n).
   *
   *
   * @param node element node of the interest. Cannot be nullptr.
   * @return size_t element number which corresponds to the given node. Note
   * that unlike the index element number numeration start from 1.
   */
  static size_t GetElementNumber(const Node* node) {
    assert(node);
    size_t curr_num = GetTreeSize(node->left) + 1;
    const Node* parent = node->parent;
    while (parent) {
      if (parent->right == node) {
        curr_num += GetTreeSize(parent->left) + 1;
      }
      node = parent;
      parent = parent->parent;
    }
    return curr_num;
  }

  Node* root_ = nullptr;
  std::mt19937_64 rnd_;
  size_t size_ = 0;
};

}  // namespace alpa

#endif  // INCLUDE_ALGORITHM_PACK_IMPLICIT_TREAP_H
