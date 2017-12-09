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

#if defined(WIN32GUI)

  // GUI version for windows.
#include <windows.h>
#include <commctrl.h>
#include "./win32gui.h"
#include "./resource.h"

#define MAIN int WINAPI wWinMain(\
    HINSTANCE hinstance, \
    HINSTANCE not_used, \
    LPTSTR cmdline, \
    int cmdshow)

#elif defined(WIN32)

// Console version for windows.
#define MAIN int wmain(int argc, char* argv[])

#else

// Console version for ubuntu.
#define MAIN int main(int argc, char* argv[])

#endif

MAIN {
  Data data;

#if defined(WIN32GUI) // GUI version for windows.

  // An console is allocated for stdio.
  FILE* fp = nullptr;
  AllocConsole();
  _wfreopen_s(&fp, L"CONOUT$", L"w", stdout);
  _wfreopen_s(&fp, L"CONOUT$", L"w", stderr);
  _wfreopen_s(&fp, L"CONIN$", L"r", stdin);

  // The dialog is opened.
  DialogBoxParam(
      hinstance,
      MAKEINTRESOURCE(IDD_MAINPANEL),
      nullptr,
      &DialogProc,
      reinterpret_cast<LPARAM>(&data));

  // The console is released.
  FreeConsole();

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(not_used);
  UNREFERENCED_PARAMETER(cmdline);
  UNREFERENCED_PARAMETER(cmdshow);

#else // Console version for windows and ubuntu.

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

#endif

  return 0;
}
