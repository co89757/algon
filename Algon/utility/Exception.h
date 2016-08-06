#pragma once
#include <stdexcept>
#include "Util.h"
#include <cstdarg>
#include "../common.h"
#define NEW_RTEXCEPTION(E) class E: public std::runtime_error\
    {\
  public:\
   explicit E(const char* m): runtime_error(m){}\
   explicit E(const std::string& m): runtime_error(m){}\
   virtual ~E(){}\
    }
#define NEW_EXCEPTION(E,STDBASE) class E:public std::STDBASE\
  {\
   public:\
    explicit E(const char* m): STDBASE(m){}\
    explicit E(const std::string& m): STDBASE(m){}\
    virtual ~E(){}\
  }

namespace colinli {

  NEW_RTEXCEPTION(NullArgumentError);

  NEW_RTEXCEPTION(FileIOException);

  NEW_RTEXCEPTION(InvalidDataException);

  NEW_RTEXCEPTION(ApplicationException);

  NEW_RTEXCEPTION(UnsupportedException);

  template<typename TExcept, typename ... TArgs>
  void Throw(TArgs&&... rest) {

    throw TExcept(ToStr(std::forward<TArgs>(rest)...).c_str());
  }

  template<typename TExcept, typename TVal, typename ... TRest>
  void ThrowOnFalse(TVal&& value, TRest&&... rest) {
    if (!value) {
      Throw<TExcept>(std::forward<TRest>(rest)...);
    }
  }

  template<typename E>
  void Throwf(const char* fmt, ...) {
    char buf[DEFAULT_BUFSIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    throw E(std::string(buf).c_str());
  }

}// end of colinli ns


#define THROW_FMT(EXCEPTION, fmt, ...) \
  ::colinli::Throwf<EXCEPTION>("[Error] %s:%d:%s:\n" fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__ )

#define CHECK_THROW(cond,E) \
  if(!(cond)) \
   ::colinli::Throwf<E>("[CheckFailure] %s:%d:%s:\n" "Failed Condition: " #cond , __FILE__,__LINE__,__func__ )
  