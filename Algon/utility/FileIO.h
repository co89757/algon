#pragma once
#include <stdio.h>
#include "Exception.h"
#include "../common.h"
namespace colinli {
namespace file {
  using colinli::uPtr;



class ReadableFile
{
public:
  explicit ReadableFile(const char* name) :name_(name) {
    fp = fopen(name, "rb+");
    if (!fp) {
      Throwf<FileIOException>("File %s cannot be opened, check existence or permission\n", name);
    }
  }

  bool ReadBytes(char* buffer, size_t n);

  uPtr<std::string> ReadLine() const;
  bool ReadLine(std::string& line);

  virtual ~ReadableFile() {
    if (fp) {
      fclose(fp);
    }
  }

  bool Skip(size_t n) {
    if (fseek(fp, n, SEEK_CUR)!=0) {
      return false;
    }
    return true;
  }
  
private:
  FILE* fp;
  const char* name_;
};

} // end of file ns
}//end of colinli ns 