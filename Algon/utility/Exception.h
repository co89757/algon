#pragma once
#include <stdexcept>
#include <string>
#include "Util.h"
#include <cstdarg>
#include "../common.h"

#ifdef ALGON_IS_GNU_LINUX
#include <execinfo.h>
#include <cxxabi.h>

/** Print a demangled stack backtrace of the caller function to FILE* out. */
static inline void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63)
{
    fprintf(out, "stack trace:\n");

    // storage array for stack trace address data
    void* addrlist[max_frames+1];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
	fprintf(out, "  <empty, possibly corrupt>\n");
	return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++)
    {
	char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

	// find parentheses and +address offset surrounding the mangled name:
	// ./module(function+0x15c) [0x8048a6d]
	for (char *p = symbollist[i]; *p; ++p)
	{
	    if (*p == '(')
		begin_name = p;
	    else if (*p == '+')
		begin_offset = p;
	    else if (*p == ')' && begin_offset) {
		end_offset = p;
		break;
	    }
	}

	if (begin_name && begin_offset && end_offset
	    && begin_name < begin_offset)
	{
	    *begin_name++ = '\0';
	    *begin_offset++ = '\0';
	    *end_offset = '\0';

	    // mangled name is now in [begin_name, begin_offset) and caller
	    // offset in [begin_offset, end_offset). now apply
	    // __cxa_demangle():

	    int status;
	    char* ret = abi::__cxa_demangle(begin_name,
					    funcname, &funcnamesize, &status);
	    if (status == 0) {
		funcname = ret; // use possibly realloc()-ed string
		fprintf(out, "  %s : %s+%s\n",
			symbollist[i], funcname, begin_offset);
	    }
	    else {
		// demangling failed. Output function name as a C function with
		// no arguments.
		fprintf(out, "  %s : %s()+%s\n",
			symbollist[i], begin_name, begin_offset);
	    }
	}
	else
	{
	    // couldn't parse the line? print the whole line.
	    fprintf(out, "  %s\n", symbollist[i]);
	}
    }

    free(funcname);
    free(symbollist);
}
#endif 



#define NEW_RTEXCEPTION(E) class E: public std::runtime_error\
    {\
  public:\
   explicit E(const char* m): runtime_error(m){}\
   explicit E(const std::string& m): runtime_error(m){}\
   virtual ~E(){}\
    }
#define NEW_EXCEPTION(E,STDBASE) class E:public STDBASE\
  {\
   public:\
    explicit E(const char* m): STDBASE(m){}\
    explicit E(const std::string& m): STDBASE(m){}\
    virtual ~E(){}\
  }

namespace colinli {

  class Exception: public std::exception{
  public:
    explicit Exception(const char* m):msg(m) {}
    explicit Exception(const std::string& m):msg(m) {}
    virtual const char* what() override {
      return msg.c_str();
    }
    virtual void PrintTrace() { _printtrace(); }
    virtual std::string& StackTrace() { //TODO }
    virtual ~Exception(){}
  private:
    std::string msg; 
    void _printtrace(){
    //TODO 
    }
  };



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
  