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
#include <sat/v1.0.2/data.h>
#include <sat/v1.0.2/util.h>
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

  // The number of span of each event.
#define SPAN_NUM      (4)

namespace {
struct ControlId {
  // ID for edit boxes.
  int edit_year[SPAN_NUM];
  int edit_mon[SPAN_NUM];
  int edit_date[SPAN_NUM];
  int edit_hour[SPAN_NUM];
  int edit_min[SPAN_NUM];
  int edit_len[SPAN_NUM];
  // ID for spins.
  int spin_year[SPAN_NUM];
  int spin_mon[SPAN_NUM];
  int spin_date[SPAN_NUM];
  int spin_hour[SPAN_NUM];
  int spin_min[SPAN_NUM];
  int spin_len[SPAN_NUM];
  // ID for buttons.
  int check_use_span[SPAN_NUM];
  // Control ids in resource script is copied to these variables.
  void Initialize() {
    // ID for edit boxes.
    edit_year[0] = IDC_ED_YR1; edit_year[1] = IDC_ED_YR2;
    edit_year[2] = IDC_ED_YR3; edit_year[3] = IDC_ED_YR4;
    edit_mon[0] = IDC_ED_MON1; edit_mon[1] = IDC_ED_MON2;
    edit_mon[2] = IDC_ED_MON3; edit_mon[3] = IDC_ED_MON4;
    edit_date[0] = IDC_ED_DATE1; edit_date[1] = IDC_ED_DATE2;
    edit_date[2] = IDC_ED_DATE3; edit_date[3] = IDC_ED_DATE4;
    edit_hour[0] = IDC_ED_HR1; edit_hour[1] = IDC_ED_HR2;
    edit_hour[2] = IDC_ED_HR3; edit_hour[3] = IDC_ED_HR4;
    edit_min[0] = IDC_ED_MIN1; edit_min[1] = IDC_ED_MIN2;
    edit_min[2] = IDC_ED_MIN3; edit_min[3] = IDC_ED_MIN4;
    edit_len[0] = IDC_ED_LEN1; edit_len[1] = IDC_ED_LEN2;
    edit_len[2] = IDC_ED_LEN3; edit_len[3] = IDC_ED_LEN4;
    // ID for spins.
    spin_year[0] = IDC_SP_YR1; spin_year[1] = IDC_SP_YR2;
    spin_year[2] = IDC_SP_YR3; spin_year[3] = IDC_SP_YR4;
    spin_mon[0] = IDC_SP_MON1; spin_mon[1] = IDC_SP_MON2;
    spin_mon[2] = IDC_SP_MON3; spin_mon[3] = IDC_SP_MON4;
    spin_date[0] = IDC_SP_DATE1; spin_date[1] = IDC_SP_DATE2;
    spin_date[2] = IDC_SP_DATE3; spin_date[3] = IDC_SP_DATE4;
    spin_min[0] = IDC_SP_MIN1; spin_min[1] = IDC_SP_MIN2;
    spin_min[2] = IDC_SP_MIN3; spin_min[3] = IDC_SP_MIN4;
    spin_hour[0] = IDC_SP_HR1; spin_hour[1] = IDC_SP_HR2;
    spin_hour[2] = IDC_SP_HR3; spin_hour[3] = IDC_SP_HR4;
    spin_len[0] = IDC_SP_LEN1; spin_len[1] = IDC_SP_LEN2;
    spin_len[2] = IDC_SP_LEN3; spin_len[3] = IDC_SP_LEN4;
    // ID for buttons.
    check_use_span[0] = IDC_CB_EV1; check_use_span[1] = IDC_CB_EV2;
    check_use_span[2] = IDC_CB_EV3; check_use_span[3] = IDC_CB_EV4;
  }
};
struct WindowHandle {
  HWND event_tab;
  HWND event_dialog;
  HWND main_dialog;
};
struct EventData {
  int use_span[SPAN_NUM];  // Used as a flag.
  int len[SPAN_NUM];
  sat::Calendar cal[SPAN_NUM];
};

  //
  // Variables.
  //
Data* data = nullptr;
ControlId control_id;
WindowHandle handle;
std::vector<EventData> event_data;

  //
  // Functions used in message crackers.
  //
void Message(HWND hwnd, const wchar_t* message) {
  assert(message);
  MessageBox(hwnd, message, L"wsatpass", MB_OK);
}
int GetEditValue(HWND hwnd, int edit_id) {
  wchar_t buf[16] = {0};
  GetDlgItemText(hwnd, edit_id, buf, sizeof(buf));
  return _wtoi(buf);
}
void SetEditValue(HWND hwnd, int edit_id, int value) {
  wchar_t buf[16] = {0};
  swprintf_s(buf, 10, L"%d", value);
  SetDlgItemText(hwnd, edit_id, buf);
}
void ChangeEditBySpin(HWND hwnd, int edit_id, const NMUPDOWN& spin) {
  if (spin.hdr.code == UDN_DELTAPOS) {
    const int value = GetEditValue(hwnd, edit_id);
    if (spin.iDelta < 0) {
      SetEditValue(hwnd, edit_id, value + 1);
    } else if (spin.iDelta > 0) {
      SetEditValue(hwnd, edit_id, value - 1);
    }
  }
}
void GetEventDialog(HWND hwnd, EventData* event, int spin_id) {
  assert(event);
  // Edit values are get.
  event->cal[spin_id].year = GetEditValue(hwnd, control_id.edit_year[spin_id]);
  event->cal[spin_id].mon = GetEditValue(hwnd, control_id.edit_mon[spin_id]);
  event->cal[spin_id].day = GetEditValue(hwnd, control_id.edit_date[spin_id]);
  event->cal[spin_id].hour = GetEditValue(hwnd, control_id.edit_hour[spin_id]);
  event->cal[spin_id].min = GetEditValue(hwnd, control_id.edit_min[spin_id]);
  event->len[spin_id] = GetEditValue(hwnd, control_id.edit_len[spin_id]);
  // Radio buttons are get.
  if (Button_GetCheck(GetDlgItem(hwnd, control_id.check_use_span[spin_id])) ==
      BST_CHECKED) {
    event->use_span[spin_id] = TRUE;
  } else {
    event->use_span[spin_id] = FALSE;
  }
}
void SetEventDialog(HWND hwnd, const EventData& event, int span_id) {
  // Edit status are set.
  int is_used = event.use_span[span_id];
  Edit_Enable(GetDlgItem(hwnd, control_id.edit_year[span_id]), is_used);
  Edit_Enable(GetDlgItem(hwnd, control_id.edit_mon[span_id]), is_used);
  Edit_Enable(GetDlgItem(hwnd, control_id.edit_date[span_id]), is_used);
  Edit_Enable(GetDlgItem(hwnd, control_id.edit_hour[span_id]), is_used);
  Edit_Enable(GetDlgItem(hwnd, control_id.edit_min[span_id]), is_used);
  Edit_Enable(GetDlgItem(hwnd, control_id.edit_len[span_id]), is_used);
  // Edit values are set.
  SetEditValue(hwnd, control_id.edit_year[span_id], event.cal[span_id].year);
  SetEditValue(hwnd, control_id.edit_mon[span_id], event.cal[span_id].mon);
  SetEditValue(hwnd, control_id.edit_date[span_id], event.cal[span_id].day);
  SetEditValue(hwnd, control_id.edit_hour[span_id], event.cal[span_id].hour);
  SetEditValue(hwnd, control_id.edit_min[span_id], event.cal[span_id].min);
  SetEditValue(hwnd, control_id.edit_len[span_id], event.len[span_id]);
  // Buttons are set.
  if (event.use_span[span_id] == TRUE) {
    Button_SetCheck(GetDlgItem(hwnd, control_id.check_use_span[span_id]),
        BST_CHECKED);
  } else {
    Button_SetCheck(GetDlgItem(hwnd, control_id.check_use_span[span_id]),
        BST_UNCHECKED);
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
  int tab_index = TabCtrl_GetCurFocus(handle.event_tab);
  switch (id) {
    case IDC_CB_EV1:
      GetEventDialog(hwnd, &event_data[tab_index], 0);
      SetEventDialog(hwnd, event_data[tab_index], 0);
      break;
    case IDC_CB_EV2:
      GetEventDialog(hwnd, &event_data[tab_index], 1);
      SetEventDialog(hwnd, event_data[tab_index], 1);
      break;
    case IDC_CB_EV3:
      GetEventDialog(hwnd, &event_data[tab_index], 2);
      SetEventDialog(hwnd, event_data[tab_index], 2);
      break;
    case IDC_CB_EV4:
      GetEventDialog(hwnd, &event_data[tab_index], 3);
      SetEventDialog(hwnd, event_data[tab_index], 3);
      break;
    case IDC_ALLLEN:
      {
        int event_id = TabCtrl_GetCurSel(handle.event_tab);
        // The event span is reset.
        SYSTEMTIME today;
        HWND hwnd_cal = GetDlgItem(handle.main_dialog, IDC_CAL_LEN);
        MonthCal_GetToday(hwnd_cal, &today);
        for (int span_id = 0; span_id < SPAN_NUM; ++span_id) {
          event_data[event_id].use_span[span_id] = FALSE;
          event_data[event_id].len[span_id] = 0;
          event_data[event_id].cal[span_id].year = today.wYear;
          event_data[event_id].cal[span_id].mon = today.wMonth;
          event_data[event_id].cal[span_id].day = today.wDay;
        }
        // The all span is set for span 1.
        SYSTEMTIME range[2] = {0};
        MonthCal_GetSelRange(hwnd_cal, range);
        sat::Calendar cal_start;
        cal_start.year = range[0].wYear;
        cal_start.mon = range[0].wMonth;
        cal_start.day = range[0].wDay;
        cal_start.hour = cal_start.min = 0;
        cal_start.sec = 0.0;
        double start = sat::ToJulianDay(cal_start);
        sat::Calendar cal_stop;
        cal_stop.year = range[1].wYear;
        cal_stop.mon = range[1].wMonth;
        cal_stop.day = range[1].wDay;
        cal_stop.hour = 23;
        cal_stop.min = 59;
        cal_stop.sec = 59.999;
        double stop = sat::ToJulianDay(cal_stop);
        // Data is set to span 1.
        event_data[event_id].use_span[0] = TRUE;
        event_data[event_id].cal[0] = cal_start;
        event_data[event_id].len[0] = static_cast<int>((stop - start) * 1440);
        // The data is reflected.
        for (int span_id = 0; span_id < SPAN_NUM; ++span_id) {
          SetEventDialog(handle.event_dialog, event_data[event_id], span_id);
        }
      }
      break;
    default:
      // No implementation.
      break;
  }
  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(id);
  UNREFERENCED_PARAMETER(hwnd_ctrl);
  UNREFERENCED_PARAMETER(code_notify);
}
void OnNotify(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  NMUPDOWN nmupdown = *reinterpret_cast<NMUPDOWN*>(lp);
  switch (wp) {
    // Event year
    case IDC_SP_YR1: ChangeEditBySpin(hwnd, IDC_ED_YR1, nmupdown); break;
    case IDC_SP_YR2: ChangeEditBySpin(hwnd, IDC_ED_YR2, nmupdown); break;
    case IDC_SP_YR3: ChangeEditBySpin(hwnd, IDC_ED_YR3, nmupdown); break;
    case IDC_SP_YR4: ChangeEditBySpin(hwnd, IDC_ED_YR4, nmupdown); break;
    // Event month
    case IDC_SP_MON1: ChangeEditBySpin(hwnd, IDC_ED_MON1, nmupdown); break;
    case IDC_SP_MON2: ChangeEditBySpin(hwnd, IDC_ED_MON2, nmupdown); break;
    case IDC_SP_MON3: ChangeEditBySpin(hwnd, IDC_ED_MON3, nmupdown); break;
    case IDC_SP_MON4: ChangeEditBySpin(hwnd, IDC_ED_MON4, nmupdown); break;
    // Event date
    case IDC_SP_DATE1: ChangeEditBySpin(hwnd, IDC_ED_DATE1, nmupdown); break;
    case IDC_SP_DATE2: ChangeEditBySpin(hwnd, IDC_ED_DATE2, nmupdown); break;
    case IDC_SP_DATE3: ChangeEditBySpin(hwnd, IDC_ED_DATE3, nmupdown); break;
    case IDC_SP_DATE4: ChangeEditBySpin(hwnd, IDC_ED_DATE4, nmupdown); break;
    // Event hour
    case IDC_SP_HR1: ChangeEditBySpin(hwnd, IDC_ED_HR1, nmupdown); break;
    case IDC_SP_HR2: ChangeEditBySpin(hwnd, IDC_ED_HR2, nmupdown); break;
    case IDC_SP_HR3: ChangeEditBySpin(hwnd, IDC_ED_HR3, nmupdown); break;
    case IDC_SP_HR4: ChangeEditBySpin(hwnd, IDC_ED_HR4, nmupdown); break;
    // Event minute
    case IDC_SP_MIN1: ChangeEditBySpin(hwnd, IDC_ED_MIN1, nmupdown); break;
    case IDC_SP_MIN2: ChangeEditBySpin(hwnd, IDC_ED_MIN2, nmupdown); break;
    case IDC_SP_MIN3: ChangeEditBySpin(hwnd, IDC_ED_MIN3, nmupdown); break;
    case IDC_SP_MIN4: ChangeEditBySpin(hwnd, IDC_ED_MIN4, nmupdown); break;
    // Event span
    case IDC_SP_LEN1: ChangeEditBySpin(hwnd, IDC_ED_LEN1, nmupdown); break;
    case IDC_SP_LEN2: ChangeEditBySpin(hwnd, IDC_ED_LEN2, nmupdown); break;
    case IDC_SP_LEN3: ChangeEditBySpin(hwnd, IDC_ED_LEN3, nmupdown); break;
    case IDC_SP_LEN4: ChangeEditBySpin(hwnd, IDC_ED_LEN4, nmupdown); break;
    default:
      // No implementation.
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
      // No implementation.
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

  // Controls are initialized.
  InitCommonControls();
  control_id.Initialize();
  handle.main_dialog = hwnd;

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
  HWND hwnd_cal = GetDlgItem(hwnd, IDC_CAL_LEN);
  MonthCal_SetMaxSelCount(hwnd_cal, SATPASS_MAX_DAYS);
  MonthCal_SetMonthDelta(hwnd_cal, 1);

  // The tab control is initialized.
  // The event dialog is pasted on the tab control.
  handle.event_tab = GetDlgItem(hwnd, IDC_TAB_EV);
  TCITEM tcitem;
  tcitem.mask = TCIF_TEXT;
  for (int event_id = 0; event_id < static_cast<int>(data->events.size());
      ++event_id) {
    tcitem.pszText = (LPWSTR) data->events[event_id].c_str();
    TabCtrl_InsertItem(handle.event_tab, event_id, &tcitem);
  }
  handle.event_dialog = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_EVENTSPAN),
      handle.event_tab, event_dialog::DialogProc);
  RECT rc;
  GetWindowRect(handle.event_tab, &rc);
  MoveWindow(handle.event_dialog, 0, 20, (rc.right - rc.left),
      (rc.bottom - rc.top), TRUE);

  // The initial input TZ check is set.
  Button_SetCheck(GetDlgItem(hwnd, IDC_RB_JST), BST_CHECKED);

  // The initial output TZ check is set.
  Button_SetCheck(GetDlgItem(hwnd, IDC_RB_UT2), BST_CHECKED);

  // The initial output file format check is set.
  Button_SetCheck(GetDlgItem(hwnd, IDC_RB_TEXT), BST_CHECKED);

  // The event span data is initialized according to today's date.
  event_data.resize(data->events.size());
  SYSTEMTIME today;
  MonthCal_GetToday(GetDlgItem(hwnd, IDC_CAL_LEN), &today);
  for (int event_id = 0; event_id < static_cast<int>(data->events.size());
      ++event_id) {
    for (int span_id = 0; span_id < SPAN_NUM; ++span_id) {
      event_data[event_id].cal[span_id].year = today.wYear;
      event_data[event_id].cal[span_id].mon = today.wMonth;
      event_data[event_id].cal[span_id].day = today.wDay;
      event_data[event_id].cal[span_id].hour = 0;
      event_data[event_id].cal[span_id].min = 0;
      event_data[event_id].cal[span_id].sec = 0;
      event_data[event_id].len[span_id] = 0;
      event_data[event_id].use_span[span_id] = FALSE;
      // Changes are reflected to controls.
      SetEventDialog(handle.event_dialog, event_data[event_id], span_id);
    }
  }

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
      } else {
        Message(hwnd, L"File is exported");
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
void OnNotify(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  assert(lp);
  NMHDR* nmhdr = reinterpret_cast<NMHDR*>(lp);
  int event_id = 0;
  switch (nmhdr->code){
    case TCN_SELCHANGING:
      event_id = TabCtrl_GetCurSel(handle.event_tab);
      for (int span_id = 0; span_id < SPAN_NUM; ++span_id) {
        GetEventDialog(handle.event_dialog, &event_data[event_id], span_id);
      }
      break;
    case TCN_SELCHANGE:
      event_id = TabCtrl_GetCurSel(handle.event_tab);
      for (int span_id = 0; span_id < SPAN_NUM; ++span_id) {
        SetEventDialog(handle.event_dialog, event_data[event_id], span_id);
      }
      break;
    default:
      // No implementation.
      break;
  }
  // Warnings are prevented for non-used parameters.
  UNREFERENCED_PARAMETER(hwnd);
  UNREFERENCED_PARAMETER(msg);
  UNREFERENCED_PARAMETER(wp);
}
}  // main_dialog
}  // namespace

INT_PTR CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
    HANDLE_DLG_MSG(hwnd, WM_INITDIALOG, main_dialog::OnCreate);
    HANDLE_DLG_MSG(hwnd, WM_DESTROY, main_dialog::OnDestroy);
    HANDLE_DLG_MSG(hwnd, WM_COMMAND, main_dialog::OnCommand);
    HANDLE_DLG_MSG(hwnd, WM_CLOSE, main_dialog::OnClose);
    case WM_NOTIFY:
      main_dialog::OnNotify(hwnd, msg, wp, lp);
      return FALSE;
    default:
      // No implementation.
      return FALSE;
  }
}
