#pragma once
#include <vector>
#include <type_traits>
#include <assert.h>
#include <cstdint>
#include "../common.h"
#define DELETE(x) do{ \
delete (x);\
x = 0;\
}while(0)
namespace colinli {
namespace algon {

template <typename TKey, typename TVal, int MAXHEIGHT>
struct SkipNode
{
  TKey Key;
  TVal Value;
  SkipNode* Forwards[MAXHEIGHT + 1];
  size_t Height;

  SkipNode(TKey k, TVal v, size_t h) :
    Key(k),
    Value(v),
    Height(h) 
    {
    assert(h <= MAXHEIGHT);
    for (int h = MAXHEIGHT; h > 0; --h) {
      Forwards[h] = NULL;
    }
   }

  SkipNode(typename std::enable_if<std::is_default_constructible<TKey>::value, TKey>::type* dummy = 0):
    Key(TKey()),
    Value(TVal()),
    Height(1) {
    for (int h = MAXHEIGHT; h > 0; --h) {
      Forwards[h] = NULL;
    }
  }

  virtual ~SkipNode() { }
};

template <typename TKey, typename TVal, int MAXHEIGHT>
class SkipList
{
public:
  typedef SkipNode<TKey, TVal, MAXHEIGHT> TNode;

  explicit SkipList() : curHeight_(1), count_(0) {
    head_ = new SkipNode<TKey, TVal, MAXHEIGHT>();
    tail_ = new SkipNode<TKey, TVal, MAXHEIGHT>();
    for (int i = MAXHEIGHT; i > 0; --i) {
      head_->Forwards[i] = tail_;
    }

  }

  virtual ~SkipList() {
    TNode* cursor = head_->Forwards[1];
    while (cursor->Forwards[1] != tail_) {
      TNode* next = cursor->Forwards[1];
      delete cursor;
      cursor = next;
    }
    delete head_;
    delete tail_;
  }

  /// <summary>
  /// Get the value of a given key only when the key is present 
  /// </summary>
  /// <param name="key">The key.</param>
  /// <param name="value">The pointer to the value returned. This value will be populated only when key is found</param>
  /// <returns>true iff the key is present in the list</returns>
  bool TryGet(TKey key, TVal* value) {
    if (!value) {
      return false;
    }
    TNode* cursor = head_;
    for (int lvl = curHeight_; lvl > 0; lvl--) {
      while (cursor->Forwards[lvl] != tail_ && cursor->Forwards[lvl]->Key < key) {
        cursor = cursor->Forwards[lvl];
      }

    }
    cursor = cursor->Forwards[1];
    if (cursor->Key == key) {
      *value = cursor->Value;
      return true;
    }
    return false;
  }

  /// <summary>
  /// Puts the specified key and value. If the key is already present, update the value
  /// </summary>
  /// <param name="key">The key.</param>
  /// <param name="value">The value.</param>
  void Put(TKey key, TVal value) {
    bool existkey = false;
    TNode* update[MAXHEIGHT + 1];
    TNode* cursor = head_;
    //first find the key 
    for (int h = curHeight_; h > 0; h--) {
      while (cursor->Forwards[h] != tail_ && cursor->Forwards[h]->Key < key) {
        cursor = cursor->Forwards[h];
      }
      update[h] = cursor;
    }
    if (cursor->Forwards[1]->Key == key) {
      existkey = true;
    }
    if (existkey) {
      for (int h = curHeight_; h > 0; h--) {
        if (update[h]->Forwards[h] != tail_) {
          //update value 
          update[h]->Forwards[h]->Value = value;
        }

      }
    }
    else {
      //insert new node 
      auto newLevel = getRandomHeight();
      TNode* newNode = new TNode(key, value, newLevel);
      if (newLevel > curHeight_) {
        for (int h = newLevel; h > curHeight_; h--) {
          update[h] = head_;
        }
        curHeight_ = newLevel;
      }
      for (int h = newLevel; h > 0; h--) {

        newNode->Forwards[h] = update[h]->Forwards[h];
        update[h]->Forwards[h] = newNode;
      }
      // update count 
      ++count_;
    }

  }

  bool HasKey(TKey key) {
    TVal v;
    bool haskey = TryGet(key, &v);
    return haskey;
  }

  /// <summary>
  /// Tries to remove the given key only when it is present
  /// </summary>
  /// <param name="key">The key.</param>
  /// <returns>true when the key is found and successfully deleted</returns>
  bool TryRemove(TKey key) {
    TNode* cursor = head_;
    TNode* update[MAXHEIGHT + 1];
    for (int h = curHeight_; h > 0; --h) {
      while (cursor->Forwards[h] != tail_ && cursor->Forwards[h]->Key < key) {
        cursor = cursor->Forwards[h];
      }
      update[h] = cursor;
    }
    if (cursor->Forwards[1]->Key != key) {
      return false;
    }
    cursor = cursor->Forwards[1]; // the node to delete
    //remove the key 
    for (int h = 1; h < curHeight_ + 1; ++h) {
      if (update[h]->Forwards[h]->Key != key) {
        break;
      }
      //relink the pointers
      update[h]->Forwards[h] = cursor->Forwards[h];
    }
    delete cursor;
    //update height
    while (curHeight_ > 1 && head_->Forwards[curHeight_] == tail_)
      curHeight_--;

    return true;
  }

  size_t Count() const {
    return count_;
  }

  bool Empty() const {
    return count_ == 0;
  }

  void Swap(SkipList& other) {
    std::swap(this->head_, other->head_);
    std::swap(this->tail_, other->tail_);
    std::swap(count_, other->count_);
    std::swap(curHeight_, other->curHeight_);
  }

private:
  SkipNode<TKey, TVal, MAXHEIGHT>* head_;
  SkipNode<TKey, TVal, MAXHEIGHT>* tail_;
  uint8_t curHeight_;
  size_t count_;

protected:
  static double getUniformRandom() {
    return rand() / double(RAND_MAX);
  }

  static int getRandomHeight() {
    double p = 0.5;
    int level = 1;
    while (level < MAXHEIGHT && getUniformRandom() < p) {
      ++level;
    }
    return level;

  }
};

}//end of algon namespae 
} // end of colinli namespace
