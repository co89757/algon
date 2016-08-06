#pragma once
#include <memory>
#define DEFAULT_BUFSIZE 512
#if defined(_WIN32) || defined(_MSC_VER)
#define snprintf _snprintf
#define __func__ __FUNCTION__ 
#endif

#define NAMEOF(x) #x 

namespace colinli {
template<class T>
using uPtr = std::unique_ptr<T>;
template<class T>
using sPtr = std::shared_ptr<T>;
}
