#pragma once
#include <stdio.h>
#include "Exception.h"
#include "../common.h"
namespace colinli {
namespace file {
using colinli::uPtr;

class WritableFile
{
public:
  WritableFile(const char* name) :name_(name) {
    fp_ = fopen(name, "wb+");
    if (fp_ == NULL) {
      Throwf<FileIOException>("[IOError] The file %s cannot be opened for write", name);
    }
  }

  void WriteLine(const std::string& line) {
    auto r = fputs(line.c_str(), fp_);
    if (!r) {
      Throwf<FileIOException>("[WriteLine] Fail to write line for file %s", name_);
    }
    fputc('\n', fp_);
  }
  void WriteAllText(const char* txt) {
    if (!fputs(txt,fp_)) {
      Throwf<FileIOException>("[WriteAllText] Fail to write text for file %s", name_);
    }
  }
  void Flush() {
    fflush(fp_);
  }

  virtual ~WritableFile() {
    if (fp_) {
      fclose(fp_);
    }
  }
private:
  FILE* fp_;
  const char* name_;
};

class AppendableFile
{
public:
  AppendableFile(const char* name) :name_(name) {
    fp_ = fopen(name, "a");
    if (fp_ == NULL) {
      THROW_FMT(FileIOException,"Cannot open file %s for append", name);
    }
  }
  void AppendLine(const std::string& line) {
    auto r = fputs(line.c_str(), fp_);
    r = fputc('\n', fp_);
    if (!r) {
      THROW_FMT(FileIOException, "Fail to put line for file %s", name_);
    }
  }
  virtual ~AppendableFile() {
    if (!fp_) {
      fclose(fp_);
    }
  }
private:
  FILE* fp_;
  const char* name_;
};


class ReadableFile
{
public:
  explicit ReadableFile(const char* name) :name_(name) {
    fp = fopen(name, "rb+");
    if (!fp) {
      THROW_FMT(FileIOException,"File %s cannot be opened, check existence or permission\n", name);
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
  void Rewind() {
    rewind(fp);
  }
  
private:
  FILE* fp;
  const char* name_;
};

} // end of file ns
}//end of colinli ns 