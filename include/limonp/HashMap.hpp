#ifndef LIMONP_HASH_MAP_HPP
#define LIMONP_HASH_MAP_HPP

#include <utility>
#include <cstdlib>
#include "NonCopyable.hpp"

namespace limonp {

static size_t PRIME_NUMBERS[] = {3, 7, 17, 37, 79, 163, 331, 
 673, 1361, 2729, 471, 10949, 
 21911, 43853, 87719, 175447, 350899,
 701819, 1403641, 2807303, 5614657, 11229331, 
 22458671, 44917381, 89834777, 179669557, 359339171, 
 718678369, 1437356741, 2147483647
};

template <class KeyT, class MappedT>
class HashMap {
 private:
  typedef std::pair<KeyT, MappedT> ValueT;

  class LightList: public NonCopyable {
   public:
    struct Node {
      ValueT value;
      struct Node* next;
    }; // struct Node
  
    LightList()
      : head_(NULL) {
    }
    ~LightList() {
      while (head_ != NULL) {
        Node* x = head_;
        head_ = head_->next;
        delete x;
      }
    }
  
    void PushFront(const ValueT& x) {
      Node* node = new Node;
      node->value = x;
      node->next = head_;
      head_ = node;
    }
   private:
    friend class HashMap;
    friend class Iterator;

    Node* head_;
  }; // class LightList

  template<class BucketsT, class NodeT>
  class Iterator {
   public:
    Iterator() {
    }

    Iterator& operator ++() {
      assert(node_ != NULL);
      node_ = node_->next;
      while (node_ == NULL && bucketid_ < maxsize_) {
        bucketid_ ++;
        node_ = buckets_[bucketid_]->head_;
      }
      return *this;
    }

    bool operator == (const Iterator& iter) const {
      return node_ == iter.node_;
    }

    bool operator != (const Iterator& iter) const {
      return !((*this) == iter);
    }

    ValueT* operator -> () {
      return &node_->value;
    }
    ValueT& operator * () {
      return node_->value;
    }
   private:
    friend class HashMap;
    Iterator(size_t bucketid, BucketsT buckets, size_t maxsize, NodeT node)
     : bucketid_(bucketid), buckets_(buckets), maxsize_(maxsize), node_(node) {
    }
    size_t bucketid_;
    BucketsT buckets_;
    size_t maxsize_;
    NodeT node_;
  }; // class Iterator
  
 public:
  typedef Iterator<LightList*, struct LightList::Node*> iterator;
  typedef Iterator<const LightList*, const struct LightList::Node*> const_iterator;

  HashMap()
   : buckets_(NULL), size_(0), max_size_(0) {
  }
  ~HashMap() {
    //TODO
  }

  MappedT& operator [](const KeyT& key) {
  }

  const_iterator Find(const KeyT& key) const {
    if (size_ == 0) {
      return End();
    }
    assert(max_size_ > 0);
    size_t bucketid = hash(key) % max_size_;
    const struct LightList::Node* node = buckets_[bucketid]->head_;
    while (node != NULL) {
      if (node->value.first == key) {
        return const_iterator(bucketid, buckets_, max_size_, node);
      }
    }
    return End();
  }

  const_iterator Begin() const {
    size_t bucketid = 0;
    const struct LightList::Node* node = buckets_[bucketid]->head_;
    node = node->next;
    while (node == NULL && bucketid < max_size_) {
      bucketid ++;
      node = buckets_[bucketid]->head_;
    }
    return const_iterator(bucketid, buckets_, max_size_, node);
  }
  const_iterator End() const {
    return const_iterator(0, NULL, 0, NULL);
  }

  void Rehash(size_t maxsize) {
    const LightList* oldbuckets = buckets_;
    LightList* newbuckets = (LightList*)malloc(maxsize * sizeof(LightList*));
    memset(newbuckets, 0, maxsize * sizeof(LightList*));
    for (size_t i = 0; i < size_; ++i) {
      struct LightList::Node* oldnode = buckets_[i]->head_;
      while (oldnode != NULL) {
        size_t bucketid = hash(oldnode->value) % maxsize; //TODO
        newbuckets[bucketid]->PushFront(oldnode->value);
      }
    }
    buckets_ = newbuckets;
    free(oldbuckets); // TODO
  }
 private:
  LightList* buckets_;
  size_t size_;
  size_t max_size_;
}; // class HashMap

} // namespace limonp

#endif // LIMONP_HASH_MAP_HPP
