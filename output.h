  // @file output.h
  // @brief Output functions.
  // @author Mamoru Kaminaga
  // @date 2017-09-11 14:42:14
  // Copyright 2017 Mamoru Kaminaga
#ifndef OUTPUT_H_
#define OUTPUT_H_
#include "./common.h"
void DisplayTLE(FILE* fp, const Data& data);
void DisplayPositions(FILE* fp, const Data& data);
void DisplayEvents(FILE* fp, const Data& data);
void DisplayDetailedEvents(FILE* fp, const Data& data);
void DisplayPasses(FILE* fp, const Data& data);
bool OutputFile(const Data& data);
#endif  // OUTPUT_H_
