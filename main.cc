  // @file main.cc
  // @brief The main function.
  // @author Mamoru Kaminaga
  // @date 2017-09-05 19:51:09
  // Copyright 2017 Mamoru Kaminaga
#include <wchar.h>
#include "./common.h"
#include "./file.h"
#include "./input.h"
#include "./output.h"
#include "./solution.h"
#ifdef WIN32
int wmain(int argc, char* argv[]) {
#else
int main(int argc, char* argv[]) {
#endif
  Data data;
  // The TLE is read and displayed.
  if (!ReadTLEData(L"TLE.txt", &data)) return -1;
  DisplayTLE(stdout, data);

  // Positions are read and displayed.
  if (!ReadPositions(L"POS.txt", &data)) return -1;
  DisplayPositions(stdout, data);

  // The modes are read and read.
  if (!ReadEvent(L"EVENT.txt", &data)) return -1;
  DisplayEvents(stdout, data);

  // The input is processed.
  if (!GetInput(&data)) return 0;  // The user cancel case is not an error.

  // The solution is run.
  if (!Solve(&data)) return -1;

  // The simulation result is displayed.
  DisplayPasses(stdout, data);

  // The output file.
  if (!OutputFile(data)) return -1;

  fflush(0);
  return 0;
}
