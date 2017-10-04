  // @file file.cc
  // @brief File io functions.
  // @author Mamoru Kaminaga
  // @date 2017-09-05 19:51:09
  // Copyright 2017 Mamoru Kaminaga
#ifndef FILE_H_
#define FILE_H_
#include <wchar.h>
#include "./common.h"
bool ReadTLEData(const wchar_t* file_name, Data* data);
bool ReadPositions(const wchar_t* file_name, Data* data);
bool ReadEvent(const wchar_t* file_name, Data* data);
#endif  // FILE_H_
