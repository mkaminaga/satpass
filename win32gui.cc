﻿  // @file win32gui.cc
  // @brief GUI for win32 application.
  // @author Mamoru Kaminaga
  // @date 2017-12-09 14:53:30
  // Copyright 2017 Mamoru Kaminaga
#include <assert.h>
#include <wchar.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <sat/v1.0.2/data.h>
#include <vector>
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
  wchar_t cmd[MAX_PATH + 16] = {0};
  swprintf_s(cmd, sizeof(cmd), L"notepad %s", file_name);

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

namespace event_dialog {
BOOL OnCreate(HWND hwnd, HWND hwnd_forcus, LPARAM lp) {
  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd);
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
  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(id);
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
      ChangeEditBySpin(hwnd, IDC_ED_YR2, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_YR3:
      ChangeEditBySpin(hwnd, IDC_ED_YR3, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_YR4:
      ChangeEditBySpin(hwnd, IDC_ED_YR4, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_MON1:
      ChangeEditBySpin(hwnd, IDC_ED_MON1, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_MON2:
      ChangeEditBySpin(hwnd, IDC_ED_MON2, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_MON3:
      ChangeEditBySpin(hwnd, IDC_ED_MON3, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_MON4:
      ChangeEditBySpin(hwnd, IDC_ED_MON4, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_DATE1:
      ChangeEditBySpin(hwnd, IDC_ED_DATE1, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_DATE2:
      ChangeEditBySpin(hwnd, IDC_ED_DATE2, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_DATE3:
      ChangeEditBySpin(hwnd, IDC_ED_DATE3, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_DATE4:
      ChangeEditBySpin(hwnd, IDC_ED_DATE4, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_HR1:
      ChangeEditBySpin(hwnd, IDC_ED_HR1, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_HR2:
      ChangeEditBySpin(hwnd, IDC_ED_HR2, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_HR3:
      ChangeEditBySpin(hwnd, IDC_ED_HR3, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_HR4:
      ChangeEditBySpin(hwnd, IDC_ED_HR4, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_MIN1:
      ChangeEditBySpin(hwnd, IDC_ED_MIN1, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_MIN2:
      ChangeEditBySpin(hwnd, IDC_ED_MIN2, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_MIN3:
      ChangeEditBySpin(hwnd, IDC_ED_MIN3, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_MIN4:
      ChangeEditBySpin(hwnd, IDC_ED_MIN4, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_SPAN1:
      ChangeEditBySpin(hwnd, IDC_ED_SPAN1, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_SPAN2:
      ChangeEditBySpin(hwnd, IDC_ED_SPAN2, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_SPAN3:
      ChangeEditBySpin(hwnd, IDC_ED_SPAN3, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    case IDC_SP_SPAN4:
      ChangeEditBySpin(hwnd, IDC_ED_SPAN4, reinterpret_cast<NMUPDOWN*>(lp));
      break;
    default:
      break;
  }

  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(msg);
}
INT_PTR CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
    HANDLE_DLG_MSG(hwnd, WM_INITDIALOG, event_dialog::OnCreate);
    HANDLE_DLG_MSG(hwnd, WM_DESTROY, event_dialog::OnDestroy);
    HANDLE_DLG_MSG(hwnd, WM_COMMAND, event_dialog::OnCommand);
    HANDLE_DLG_MSG(hwnd, WM_CLOSE, event_dialog::OnClose);
    case WM_NOTIFY:
      event_dialog::OnNotify(hwnd, msg, wp, lp);
      return FALSE;
    default:
      return FALSE;
  }
}
}  // event_dialog

namespace main_dialog {
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

  // The TLE is read and displayed.
  if (!ReadTLEData(SATPASS_TLE_FILE, data)) return -1;
  DisplayTLE(stdout, *data);

  // Positions are read and displayed.
  if (!ReadPositions(SATPASS_POS_FILE, data)) return -1;
  DisplayPositions(stdout, *data);

  // The modes are read and read.
  if (!ReadEvent(SATPASS_EVENT_FILE, data)) return -1;
  DisplayEvents(stdout, *data);

  // The calendar is initialized.
  HWND hwnd_cal = GetDlgItem(hwnd, IDC_CAL_SPAN);
  MonthCal_SetMaxSelCount(hwnd_cal, SATPASS_MAX_DAYS);
  MonthCal_SetMonthDelta(hwnd_cal, 1);

  // The tab control is initialized.
  // The event dialog is pasted on the tab control.
  HWND hwnd_tab = GetDlgItem(hwnd, IDC_TAB_EV);
  TCITEM tcitem;
  tcitem.mask = TCIF_TEXT;
  for (int i = 0; i < static_cast<int>(data->events.size()); ++i) {
    tcitem.pszText = (LPWSTR) data->events[i].c_str();
    TabCtrl_InsertItem(hwnd_tab, i, &tcitem);
  }
  HWND hwnd_event = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_EVENTSPAN),
      hwnd_tab, event_dialog::DialogProc);
  RECT rc;
  GetWindowRect(hwnd_tab, &rc);
  MoveWindow(hwnd_event, 0, 20, (rc.right - rc.left), (rc.bottom - rc.top),
      TRUE);

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
      } else {
        Message(hwnd, L"Restart to reflect changes on the TLE file");
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
      if (!Solve(data)) {
        Message(hwnd, L"Failed to calculate orbit");
        break;
      }
      DisplayPasses(stdout, *data);
      if (!OutputFile(*data)) {
        Message(hwnd, L"Failed to export file");
      }
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
}  // main_dialog
}  // namespace

INT_PTR CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
    HANDLE_DLG_MSG(hwnd, WM_INITDIALOG, main_dialog::OnCreate);
    HANDLE_DLG_MSG(hwnd, WM_DESTROY, main_dialog::OnDestroy);
    HANDLE_DLG_MSG(hwnd, WM_COMMAND, main_dialog::OnCommand);
    HANDLE_DLG_MSG(hwnd, WM_CLOSE, main_dialog::OnClose);
    default:
      return FALSE;
  }
}
