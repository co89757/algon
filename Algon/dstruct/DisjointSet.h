#pragma once
#include <set>
#include <memory>
#include <unordered_map>
#include <initializer_list>
#include <assert.h>
namespace colinli {
namespace algon {

  template<typename T>
  using Set = std::set < T > ;
  template <typename K, typename V>
  using HashMap = std::unordered_map < K, V > ;

  template<typename TKey>
  class DisjointSets
  {     
  private:
    Set<TKey> keys_;
    HashMap<TKey, TKey> parent_;
    HashMap<TKey, int> rank_;
  public:
    DisjointSets(){}
    DisjointSets(std::initializer_list<TKey> keys):
      keys_(keys)
    {
      for (auto k : keys_)
      {
        parent_[k] = k;
        rank_[k] = 1;
      }    
    }    
    /// <summary>
    /// Unions two elements 
    /// </summary>
    void Union(TKey x, TKey y)
    {
      linkRoot(*FindSet(x), *FindSet(y));
    }

    bool Contains(TKey x) const 
    {
      return keys_.count(x) != 0;
    }
    
    /// <summary>
    /// Finds the set representative (root) for given element x
    /// </summary>
    /// <param name="x">input element</param>
    /// <returns>the pointer to root of the set to which x belongs</returns>
    TKey* FindSet(TKey x)
    {
      assert(parent_.count(x) != 0);
      while (parent_[x] != x)
      {
        x = *FindSet(parent_[x]);
      }
      return &x;
    }    
    /// <summary>
    /// Test connectivity of two elements
    /// </summary>
    /// <returns>true if the two elements are connected</returns>
    bool Connected(TKey x, TKey y)
    {
      if (parent_.count(x) == 0 || parent_.count(y) == 0)
      {
        return false;
      }
      return FindSet(x) == FindSet(y);
    }

    void Add(TKey x)
    {
      keys_.insert(x);
      parent_[x] = x;
      rank_[x] = 1;
    }

  private:
    // x and y are two roots to be linked
    void linkRoot(TKey x, TKey y)
    {
      assert(parent_[x] == x && parent_[y] == y);
      if (x == y)
      {
        return;
      }
      if (rank_[x] < rank_[y])
      {
        parent_[x] = y;
      }
      else
      {
        parent_[y] = x;
      }
      if (rank_[x] == rank_[y])
      {
        ++rank_[y];
      }
    }
  };
 
}//end of algon ns
}// end of colinli ns