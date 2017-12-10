  // @file win32gui.h
  // @brief GUI for win32 application.
  // @author Mamoru Kaminaga
  // @date 2017-12-09 14:53:30
  // Copyright 2017 Mamoru Kaminaga
#ifndef WIN32GUI_H_
#define WIN32GUI_H_

# if defined(WIN32GUI)

#include <wchar.h>
#include <windows.h>

INT_PTR CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

# endif

#endif  // WIN32GUI_H_
