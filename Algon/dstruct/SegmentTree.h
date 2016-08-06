#pragma once
#include <functional>
#include <iterator>
#include <vector>
#include "../utility/Exception.h"
#include "../common.h"
#include "../utility/Util.h"
namespace colinli {
namespace algon {

using std::vector;

#define LCHILD(i) (((i)<<1)+1)
#define RCHILD(i) (((i)<<1)+2)

template <typename TValue, typename TKey>
class SegmentTree
{
public:
  typedef std::function<TValue(TKey)> Transformer;
  typedef std::function<TValue(TValue, TValue)> Reducer;

  SegmentTree(vector<TKey>& collection, Reducer reducer, Transformer trans, TValue initVal):
    vector_(collection),
    N(collection.size()),
    trans_(trans),
    reducer_(reducer),
    initVal_(initVal) {
    node_values_ = new TValue[(N << 2) + 1];
    build_tree(0, N-1, reducer, trans);
  }

  void Update(size_t pos, TKey newkey) {
    update(0, 0, N - 1, pos, newkey);
  }

  TValue Query(size_t begin, size_t end) {
    return query(0, 0, N - 1, begin, end);
  }

  virtual ~SegmentTree() {
    delete[] node_values_;
  }

private:
  vector<TKey> vector_;
  size_t N;
  Transformer trans_;
  Reducer reducer_;
  TValue initVal_;
  TValue* node_values_;

  // build substree from x[begin...end] (inclusive range)
  void build_tree(size_t node_idx, size_t begin, size_t end) {
    //leaf node 
    if (begin == end) {
      node_values_[node_idx] = trans_(vector_.at(begin));
      return;
    }
    auto mid = begin + (end - begin) / 2;
    build_tree(LCHILD(node_idx), begin, mid);
    build_tree(RCHILD(node_idx), mid + 1, end);
    node_values_[node_idx] = reducer_(node_values_[LCHILD(node_idx)], node_values_[RCHILD(node_idx)]);
  }

  void update(size_t node_idx, size_t begin, size_t end, size_t pos, TKey newkey) {
    if (begin == end) {
      node_values_[node_idx] = trans_(newkey);
      return;
    }
    auto mid = begin + (end - begin) / 2;
    if (pos <= mid) {
      update(LCHILD(node_idx), begin, mid, pos, newkey);
    }
    else {
      update(RCHILD(node_idx), mid + 1, end, pos, newkey);
    }
    node_values_[node_idx] = reducer_(node_values_[LCHILD(node_idx)], node_values_[RCHILD(node_idx)]);
  }

  TValue query(size_t node_idx, size_t node_begin, size_t node_end, size_t q_begin, size_t q_end) {
    if (node_begin >= q_begin && node_end <= q_end) {
      return node_values_[node_idx];
    }
    auto mid = node_begin + (node_end - node_begin) / 2;
    TValue result = initVal_;
    if (mid > q_begin && node_begin <= q_end) {
      result = reducer_(result, query(LCHILD(node_idx), node_begin, mid, q_begin, q_end));
    }
    if (node_end >= q_begin && mid + 1 <= q_end) {
      result = reducer_(result, query(RCHILD(node_idx), mid + 1, node_end, q_begin, q_end));
    }
    return result;
  }
};

}// end of algon ns
}// end of colinli ns 

