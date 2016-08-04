#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <assert.h>
namespace colinli {
namespace file {



class Buffer
{
public:
  Buffer(const char* str) {
    assert(str);
    size_t len = strlen(str);
    data_ = new char[len+1];
    strncpy(data_, str, len);
    size_ = len;
  }
  Buffer(const char* str, size_t len) {
    assert( str && len <= strlen(str));
    data_ = new char[len+1];
    size_ = len;
    memmove(data_, str, len);
  }
  Buffer(const std::string& str) : Buffer(str.c_str()) {}
  Buffer() :data_(""), size_(0){}
  Buffer(const Buffer& other) {
    size_ = other.size_;
    data_ = new char[size_];
    memmove(data_, other.data_, size_);
  }

  Buffer& operator=(Buffer other){
    this->swap(other);
    return *this;
  }
  
  virtual ~Buffer() {
    delete[] data_;
  }
  size_t Size() const {
    return size_;
  }
  bool Empty() const {
    return size_ == 0;
  }

  char& operator[](size_t n)
  {
    assert(n < size_);
    return data_[n];
  }
  char operator[](size_t n) const
  {
    assert(n < size_);
    return data_[n];
  }
  
private:
  void swap(Buffer& other) {
    std::swap(this->data_, other.data_);
    std::swap(size_, other.size_);
  }
  char* data_;
  size_t size_;
};

}//end of file ns 
}// end of colinli ns