  // @file win32gui.cc
  // @brief GUI for win32 application.
  // @author Mamoru Kaminaga
  // @date 2017-12-09 14:53:30
  // Copyright 2017 Mamoru Kaminaga
#include <assert.h>
#include <wchar.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "./common.h"
#include "./file.h"
#include "./input.h"
#include "./output.h"
#include "./solution.h"
#include "./win32gui.h"
#include "./resource.h"

  // Message cracker wrapper for DialogProc.
#define HANDLE_DLG_MSG(hwnd, msg, fn)\
  case (msg): return SetDlgMsgResult((hwnd), (msg), \
      HANDLE_##msg((hwnd), (wp), (lp), (fn)));

namespace {
  //
  // Variables.
  //
Data* data = nullptr;

  //
  // Functions used in message crackers.
  //
void Message(HWND hwnd, const wchar_t* message) {
  assert(message);
  MessageBox(hwnd, message, L"wsatpass", MB_OK);
}
int GetEditValue(HWND hwnd, int idc_ed) {
  wchar_t buf[16] = {0};
  GetDlgItemText(hwnd, idc_ed, buf, sizeof(buf));
  return _wtoi(buf);
}
void SetEditValue(HWND hwnd, int idc_ed, int value) {
  wchar_t buf[16] = {0};
  swprintf_s(buf, 10, L"%d", value);
  SetDlgItemText(hwnd, idc_ed, buf);
}
void ChangeEditBySpin(HWND hwnd, int idc_ed, NMUPDOWN* spin) {
  assert(spin);
  if (spin->hdr.code == UDN_DELTAPOS) {
    int value = GetEditValue(hwnd, idc_ed);
    if ((spin->iDelta) < 0) {
      SetEditValue(hwnd, idc_ed, value + 1);
    } else if ((spin->iDelta) > 0) {
      SetEditValue(hwnd, idc_ed, value - 1);
    }
  }
}
bool EditFile(const wchar_t* file_name) {
  assert(file_name);
  wchar_t cmd[MAX_PATH + 8] = {0};
  swprintf_s(cmd, sizeof(cmd), L"start %s", file_name);

  // The file is edited.
  int result = 0;
  STARTUPINFO startup_info;
  ZeroMemory(&startup_info, sizeof(startup_info));
  startup_info.cb = sizeof(startup_info);
  PROCESS_INFORMATION process_info;
  ZeroMemory(&process_info, sizeof(process_info));
  result = CreateProcess(nullptr, cmd, nullptr, nullptr, FALSE,
      NORMAL_PRIORITY_CLASS, nullptr, nullptr, &startup_info,
      &process_info);
  CloseHandle(process_info.hThread);
  WaitForSingleObject(process_info.hProcess, INFINITE);
  CloseHandle(process_info.hProcess);

  if (result == 0) return false;
  return true;
}

  //
  // Message crackers.
  //
BOOL OnCreate(HWND hwnd, HWND hwnd_forcus, LPARAM lp) {
  assert(lp);
  data = reinterpret_cast<Data*>(lp);

  // The icon is loaded.
  HINSTANCE hinstance = (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);
  SendMessage(
      hwnd,
      WM_SETICON,
      ICON_BIG,
      (LPARAM) LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1)));

  // The calendar is initialized.
  HWND hwnd_cal = GetDlgItem(hwnd, IDC_CAL_SPAN);
  MonthCal_SetMaxSelCount(hwnd_cal, SATPASS_MAX_DAYS);
  MonthCal_SetMonthDelta(hwnd_cal, 1);

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd_forcus);
  UNREFERENCED_PARAMETER(lp);
  return TRUE;
}
void OnDestroy(HWND hwnd) {
  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd);
}
void OnClose(HWND hwnd) {
  // The modal dialog is ended.
  EndDialog(hwnd, TRUE);
}
void OnCommand(HWND hwnd, int id, HWND hwnd_ctrl, UINT code_notify) {
  switch (id) {
    case IDC_TLE:
      if (!EditFile(SATPASS_TLE_FILE)) {
        Message(hwnd, L"Failed to open the TLE file");
      }
      break;
    case IDC_POS:
      if (!EditFile(SATPASS_POS_FILE)) {
        Message(hwnd, L"Failed to open the POS file");
      } else {
        Message(hwnd, L"Restart to reflect changes on the position file");
      }
      break;
    case IDC_EVENT:
      if (!EditFile(SATPASS_EVENT_FILE)) {
        Message(hwnd, L"Failed to open the EVENT file");
      } else {
        Message(hwnd, L"Restart to reflect changes on the event file");
      }
      break;
    case IDC_START:
      break;
    default:
      // No implementation.
      break;
  }
  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd);
  UNREFERENCED_PARAMETER(hwnd_ctrl);
  UNREFERENCED_PARAMETER(code_notify);
}
void OnNotify(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (wp) {
    case IDC_SP_YR1:
      ChangeEditBySpin(hwnd, IDC_ED_YR1, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_YR2:
    case IDC_SP_YR3:
    case IDC_SP_YR4:
    case IDC_SP_MON1:
    case IDC_SP_MON2:
    case IDC_SP_MON3:
    case IDC_SP_MON4:
    case IDC_SP_DATE1:
    case IDC_SP_DATE2:
    case IDC_SP_DATE3:
    case IDC_SP_DATE4:
    case IDC_SP_HR1:
    case IDC_SP_HR2:
    case IDC_SP_HR3:
    case IDC_SP_HR4:
    case IDC_SP_MIN1:
    case IDC_SP_MIN2:
    case IDC_SP_MIN3:
    case IDC_SP_MIN4:
    case IDC_SP_SPAN1:
    case IDC_SP_SPAN2:
    case IDC_SP_SPAN3:
    case IDC_SP_SPAN4:
    default:
      break;
  }

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(msg);
}
}  // namespace
INT_PTR CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
    HANDLE_DLG_MSG(hwnd, WM_INITDIALOG, OnCreate);
    HANDLE_DLG_MSG(hwnd, WM_DESTROY, OnDestroy);
    HANDLE_DLG_MSG(hwnd, WM_COMMAND, OnCommand);
    HANDLE_DLG_MSG(hwnd, WM_CLOSE, OnClose);
    case WM_NOTIFY:
      OnNotify(hwnd, msg, wp, lp);
      break;
    default:
      return FALSE;
  }
  return FALSE;
}
