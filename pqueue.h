#ifndef PQUEUE_H_
#define PQUEUE_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

template <typename T, typename C = std::less<T> >
class PQueue {
 public:
  // Constructor
  PQueue() {}
  // Return number of items in priority queue
  size_t Size();
  // Return top of priority queue
  T& Top();
  // Remove top of priority queue
  void Pop();
  // Insert item and sort priority queue
  void Push(const T& item);

 private:
  std::vector<T> items;
  size_t cur_size = 0;
  C cmp;

  // Helper methods for indices
  size_t Root() { return 0; }
  size_t Parent(size_t n) { return (n - 1) / 2; }
  size_t LeftChild(size_t n) { return 2 * n + 1; }
  size_t RightChild(size_t n) { return 2 * n + 2; }

  // Helper methods for node testing
  bool HasParent(size_t n) { return n != Root(); }
  bool IsNode(size_t n) { return n < cur_size; }

  // Helper methods for restructuring
  void PercolateUp(size_t n);
  void PercolateDown(size_t n);

  // Node comparison
  bool CompareNodes(size_t i, size_t j);
};

// To be completed below

#endif  // PQUEUE_H_

// returns size of priority queue
template <typename T, typename C>
size_t PQueue<T, C>::Size() {
  return cur_size;
}

// returns top of priority queue by getting the index of the root
template <typename T, typename C>
T& PQueue<T, C>::Top() {
  assert(cur_size > 0);
  return items[Root()];
}

// pops top of priority queue by adding the last item to the root
// performs percolate down with the next root to maintain heap property
template <typename T, typename C>
void PQueue<T, C>::Pop() {
  assert(cur_size > 0);
  items[Root()] = items[cur_size - 1];
  items.pop_back();
  cur_size--;
  PercolateDown(Root());
}

// inserts item into priority queue by adding to the end of heap
// performs percolate up with the new item to maintain heap property
template <typename T, typename C>
void PQueue<T, C>::Push(const T& item) {
  items.push_back(item);
  PercolateUp(cur_size);
  cur_size++;
}

// moves item up heap by swapping with parent if order is violated
// used CompareNodes to determine if order is violated
template <typename T, typename C>
void PQueue<T, C>::PercolateUp(size_t n) {
  while (HasParent(n) && CompareNodes(Parent(n), n)) {
    std::swap(items[Parent(n)], items[n]);
    n = Parent(n);
  }
}

// moves item down heap by swapping with child if order is violated
/* checks whether to swap with left or right child
   and then determines if order is violated */
template <typename T, typename C>
void PQueue<T, C>::PercolateDown(size_t n) {
  while (IsNode(LeftChild(n))) {
    size_t child = LeftChild(n);
    if (IsNode(RightChild(n)) && !CompareNodes(RightChild(n), LeftChild(n))) {
      child = RightChild(n);
    }
    if (!CompareNodes(n, child)) {
      break;
    } else {
      std::swap(items[n], items[child]);
    }
    n = child;
  }
}

// compares two nodes to determine if order is violated
template <typename T, typename C>
bool PQueue<T, C>::CompareNodes(size_t i, size_t j) {
  if (cmp(items[i], items[j])) {
    return false;
  } else {
    return true;
  }
}
