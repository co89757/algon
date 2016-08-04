#include "FileIO.h"
#include  "Exception.h"
#include "../common.h"
using namespace colinli::file;

bool ReadableFile::ReadBytes(char* buffer, size_t n) {
  auto r = fread(buffer, 1, n, fp);
  if (r < n) {
    if (ferror(fp)) {
      return false;
    }
  }
  return true;
}

uPtr<std::string>  ReadableFile::ReadLine() const {
  uPtr<std::string> r(new std::string());
  char buffer[512];
  if (feof(fp)) {
    return nullptr;
  }
  do {
    auto buf = fgets(buffer, sizeof buffer, fp);
    if (!buf && ferror(fp)) {
      //read failure 
      Throwf<FileIOException>("[Error]");
    }
    if (feof(fp)) {
      r->append(buffer);
      return r;
    }
    if (buffer[strlen(buffer)-1] != '\n') {
      //continue to read the rest of the line
      r->append(buffer);
    }
    else {
      //read a whole line 
      buffer[strlen(buffer) - 1] = 0;
      r->append(buffer);
      break;
    }
  } while (!feof(fp));
  return r;
}

bool ReadableFile::ReadLine(std::string& line) {
  auto pStr = ReadLine();
  bool r = false;
  if (pStr) {
    r = true;
    line = *pStr;
  }
  return r;
}   



