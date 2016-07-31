#pragma once 

#include <initializer_list>
#include <vector>
#include <iostream>
#include <functional>
#include <type_traits>
#include <cassert>

#define PARENT(i) ((i)>>1)
#define LCHILD(i) ((i)<<1)

namespace colinli{
namespace algon{



    template<typename TKey, typename Comparer = std::less<TKey> >
    class Heap {
    public:
      /** default constructor*/


      Heap(const std::initializer_list <TKey> &list) {
        _init();
        for (auto key : list)
          add(key);
      }
      Heap(){
        _init();
      }

      ///template <typename TKey>
      friend
        void heapSwap(Heap<TKey, Comparer>& h1, Heap<TKey, Comparer> & h2);

      Heap<TKey>& operator=(Heap<TKey> other){
        heapSwap(*this, other);
        return *this;
      }


      void add(const TKey& key)
      {
        elems.push_back(key);
        int N = elems.size() - 1;
        //std::cout<<"N="<<N<<" \n";
        _swim(N);
        //std::cout<<"\n[DEBUG] print content: ";
        // _print();

      }

      TKey top() const
      {
        assert(elems.size() > 1);
        return elems.at(1);
      }

      size_t size() const
      {

        return elems.size() - 1;
      }
      TKey pop()
      {
        TKey top = elems.at(1);
        int N = elems.size() - 1;
        _swap(1, N);
        elems.pop_back();
        _sink(1);
        return top;

      }

      bool empty() const
      {
        return elems.size() == 1;
      }

      void poppush(TKey key)
      {
        this->pop();
        this->add(key);
      }


    private:

      ///size_t N; // index of tail element
      std::vector<TKey> elems;

      void _print(){
        for (int i = 1; i < elems.size(); i++)
          std::cout << elems.at(i) << ",";
      }

      /**
      * disuse zero index element
      */
      void _init() {
        //leave index 0 unused
        TKey _ = TKey();
        elems.push_back(_);
      }

      void _swap(int i, int j){
        assert(i >= 0 && i < elems.size());
        assert(j >= 0 && j < elems.size());
        TKey t = elems[i];
        elems[i] = elems[j];
        elems[j] = t;
      }


      void _swim(int i){
        int p;
        for (int j = i; j > 1 && less(j, (p = PARENT(j))); j = p) {
          _swap(j, p);
        }
      }

      void _sink(int i){
        int c;
        int N = elems.size() - 1;
        for (int j = i; (c = LCHILD(j)) <= N; j = c) {
          if (c + 1 <= N && less(c + 1, c))
            c++;
          if (less(j, c))
            break;
          //else swap and continue
          _swap(c, j);
        }
      }

      bool less(int me, int other){
        Comparer comp;
        return comp(elems.at(me), elems.at(other));
      }


    };

    template<typename TKey, typename Comparer>
    void heapSwap(Heap<TKey, Comparer>& h1, Heap<TKey, Comparer> & h2)
    {
      h1.elems.swap(h2.elems);
    }


    template <typename T>
    using MaxHeap = Heap<T, std::greater<T> >;

    template <typename T>
    using MinHeap = Heap<T, std::less<T> >;


    template <typename TKey, typename Less = std::less<TKey>, typename Greater = std::greater<TKey> >
    class MedianHeap
    {
    public:
      MedianHeap()
      {
        assert(minheap.empty() && maxheap.empty());
      }

      size_t size() const
      {
        return minheap.size() + maxheap.size();
      }
      void add(TKey key)
      {
        auto N = size();

        if (N % 2 == 0)
        {
          //right now the maxhp and minhp have equal #keys
          maxheap.add(key);
          if (minheap.empty())
            return;
          if (maxheap.top() > minheap.top()){
            auto toMin = maxheap.pop();
            auto toMax = minheap.pop();
            minheap.add(toMin);
            maxheap.add(toMax);

          }
        }
        else
          //maxheap has one more key, add this to minhp
          minheap.add(key);


      }

      friend
        std::istream& operator>>(std::istream& is, MedianHeap<TKey, Less, Greater> & medianhp)
      {
        TKey e;
        is >> e;
        medianhp.add(e);
        return is;
      }


      typename std::enable_if<std::is_arithmetic<TKey>::value, TKey>::type
        getMedian() const
      {

        auto N = size();


        if (N % 2 == 0)
          return (minheap.top() + maxheap.top()) / static_cast<TKey>(2);
        else
          return maxheap.top();
      }



    private:
      Heap<TKey, Less> minheap;
      Heap<TKey, Greater> maxheap;
    };

 } // end of algon namespace
}// end of colinli namespace 