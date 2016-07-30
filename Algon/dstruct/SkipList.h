#pragma once
#include <vector>
#include <type_traits>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <cstdint>

namespace colinli{
namespace algon{
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
      memset(Forwards, 0, MAXHEIGHT + 1);
    }
    SkipNode(std::enable_if_t<std::is_default_constructible<TKey>::value, TKey>* dummy = 0):
      Key(TKey()),
      Value(TVal()),
      Height(1)
      {}
    virtual ~SkipNode()
    {
      delete[] Forwards;
    }

  };
  
  template<typename TKey, typename TVal, int MAXHEIGHT>
  class SkipList
  {
  public:
    typedef SkipNode<TKey, TVal, MAXHEIGHT> TNode;

    explicit SkipList(TKey tailkey) :curHeight_(1), count_(0)
    {
      head_ = new SkipNode<TKey, TVal, MAXHEIGHT>();
      tail_ = new SkipNode<TKey, TVal, MAXHEIGHT>(tailkey, TVal(), 1);
      for (int i = MAXHEIGHT; i > 0; --i)
      {
        head_->Forwards[i] = tail_;
      }
      
    }
    bool TryGet(TKey key, TVal* value)
    {
      TNode* cursor = head_;
      for (int lvl = curHeight_; lvl > 0 ; lvl--)
      {
        while (cursor->Forwards[lvl] != tail_ && cursor->Forwards[lvl]->Key < key )
        {
          cursor = cursor->Forwards[lvl];
        }

      }
      cursor = cursor->Forwards[1];
      if (cursor->Key == key)
      {
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
    void Put(TKey key, TVal value)
    {
      bool existkey = false;
      TNode* update[MAXHEIGHT + 1];
      TNode* cursor = head_;
      //first find the key 
      for (int h = curHeight_; h > 0; h--)
      {
        while (cursor->Forwards[h] != tail_ && cursor->Forwards[h]->Key < key)
        {
          cursor = cursor->Forwards[h];
        }
        update[h] = cursor;        
      }
      if (cursor->Forwards[1]->Key == key)
      {
        existkey = true;
      }
      if (existkey)
      {
        for (int h = curHeight_; h > 0 ; h--)
        {
          if (update[h]->Forwards[h] != tail_)
          {
            //update value 
            update[h]->Forwards[h]->Value = value;
          }
          
        }
      }
      else
      {
        //insert new node 
        auto newLevel = getRandomHeight();
        TNode* newNode = new TNode(key, value, newLevel);
        if (newLevel > curHeight_)
        {
          for (int h = newLevel; h > curHeight_; h--)
          {
            update[h] = head_;
          }
          curHeight_ = newLevel;
        }
        for (int h = newLevel; h > 0; h--)
        {
          
          newNode->Forwards[h] = update[h]->Forwards[h];
          update[h]->Forwards[h] = newNode;
        }
        // update count 
        ++count_;
      }
      
    }
    bool HasKey(TKey key) 
    {
      TVal v;
      bool haskey = TryGet(key, &v);
      return haskey;
    }
    bool TryRemove(TKey key)
    {
      
    }
    size_t Count() const
    {
      return count_;
    }
    bool Empty() const
    {
      return count_ == 0;
    }
    void Swap(SkipList& other)
    {
      std::swap(this->head_, other->head_);
      std::swap(this->tail_, other->tail_);
      std::swap(count_, other->count_);
      std::swap(curHeight_, other->curHeight_);
    }
    
  private:
    SkipNode<TKey,TVal,MAXHEIGHT>* head_;
    SkipNode<TKey,TVal,MAXHEIGHT>* tail_;
    uint8_t curHeight_;
    size_t count_;
  protected:
    double getUniformRandom()
    {
      return rand() / double(RAND_MAX);
    }
    int getRandomHeight()
    {
      double p = 0.5;
      int level = 1;
      while (level < MAXHEIGHT && getUniformRandom() < p)
      {
        ++level;
      }
      return  level;

    }
  };


 }//end of algon namespae 
} // end of colinli namespace