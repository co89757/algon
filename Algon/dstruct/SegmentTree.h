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

  template<typename TValue, typename TKey>
  class SegmentTree
  {
  public:
    typedef std::function<TValue(TKey)> Transformer;
    typedef std::function<TValue(TValue, TValue)> Reducer;
    SegmentTree(vector<TKey>& collection, Reducer reducer, Transformer trans) {
      vector_ = collection;
      reducer_ = reducer;
      trans_ = trans;
      root_ = build_tree(0, collection.size(), reducer, trans);
    }
    SegmentTree(vector<TKey>&& collection, Reducer reducer, Transformer trans) {
      vector_ = collection;
      reducer_ = reducer;
      trans_ = trans;
      root_ = build_tree(0, collection.size(), reducer, trans);
    }

    struct Node
    {
      Node* left;
      Node* right;
      TValue value;
      explicit Node(TValue v) :value(v), left(nullptr), right(nullptr){}
      explicit Node() :left(nullptr), right(nullptr), value(TValue()){}
    };
  private:
    Node* root_;
    vector<TKey> vector_;
    Transformer trans_;
    Reducer reducer_;

    Node* build_tree(size_t begin,size_t end) {
      if (begin == end) {
        return NULL;
      }
      auto mid = begin + (end - begin) / 2;
      Node* node = new Node(trans_(vector_.at(mid)));
      node->left = build_tree(begin, mid);
      node->right = build_tree( std::next(mid), end);
      if (node->left && node->right) {
        node->value = reducer_(node->left->value, node->right->value);
      }
      return node;
    }

    void update(Node* node ,size_t begin, size_t end, size_t pos, TKey newkey) {
      if (begin == end || node == NULL) {
        return;
      }
      if (++begin == end) {
        node->value = trans_(newkey);
      }
      else {
        auto mid = begin + (begin - end) / 2;
        if ( pos <= mid ) {
          update(node->left, begin, mid, pos, newkey);
          update(node->right, mid+1 , end, pos, newkey);
          CHECK_THROW(node->left && node->right, NullArgumentError);
          node->value = reducer_(node->left->value, node->right->value);
        }
      }
    }
  };



}// end of algon ns
}// end of colinli ns 

