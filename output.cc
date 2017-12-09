  // @file output.h
  // @brief Output functions.
  // @author Mamoru Kaminaga
  // @date 2017-09-11 14:42:14
  // Copyright 2017 Mamoru Kaminaga
#include <assert.h>
#include <wchar.h>
#include <sat/v1.0.2/data.h>
#include <sat/v1.0.2/sat.h>
#include <sat/v1.0.2/util.h>
#include <tle/v2.1.0/tle.h>
#include <string>
#include "./common.h"
#include "./export.h"
#include "./output.h"
void DisplayTLE(FILE* fp, const Data& data) {
  assert(fp);
  fwprintf(fp, L"--------------------\n");
  fwprintf(fp, L"TLE\n");
  fwprintf(fp, L"  name %ls\n", data.tle.name.c_str());
  fwprintf(fp, L"  num %d\n", data.tle.sat_num);
  fwprintf(fp, L"  class %c\n", data.tle.classification);
  fwprintf(fp, L"  id 1 %d\n", data.tle.id_1);
  fwprintf(fp, L"  id 2 %d\n", data.tle.id_2);
  fwprintf(fp, L"  id 3 %ls\n", data.tle.id_3);
  fwprintf(fp, L"  epoch year %d\n", data.tle.epoch_year);
  fwprintf(fp, L"  epoch days %.8f\n", data.tle.epoch_days);
  fwprintf(fp, L"  ndot (rev/day^2) %.8f\n", data.tle.ndot);
  fwprintf(fp, L"  nddot (rev/day^2) %.8f\n", data.tle.nddot);
  fwprintf(fp, L"  bstar %.8f\n", data.tle.bstar);
  fwprintf(fp, L"  model %d\n", data.tle.model);
  fwprintf(fp, L"  snum %d\n", data.tle.s_num);
  fwprintf(fp, L"  incl (deg) %.8f\n", data.tle.inclo);
  fwprintf(fp, L"  node (deg) %.8f\n", data.tle.nodeo);
  fwprintf(fp, L"  eccn %.8f\n", data.tle.ecco);
  fwprintf(fp, L"  argp (deg) %.8f\n", data.tle.argpo);
  fwprintf(fp, L"  mo (deg) %.8f\n", data.tle.mo);
  fwprintf(fp, L"  no (rev/day) %.8f\n", data.tle.no);
  fwprintf(fp, L"  rev %d\n", data.tle.rev);
  fflush(0);
}
void DisplayPositions(FILE* fp, const Data& data) {
  assert(fp);
  fwprintf(fp, L"--------------------\n");
  fwprintf(fp, L"positions\n");
  for (int i = 0; i < static_cast<int>(data.nameo.size()); ++i) {
    fwprintf(fp, L"  %ls\n", data.nameo[i].c_str());
    fwprintf(fp, L"    lambda (deg), phi (deg), h (m): %04.1f %03.1f %05.1f\n",
             SAT_TO_DEGREES(data.wo[i].lambda), SAT_TO_DEGREES(data.wo[i].phi),
             data.wo[i].h);
  }
  fflush(0);
}
void DisplayEvents(FILE* fp, const Data& data) {
  assert(fp);
  fwprintf(fp, L"--------------------\n");
  fwprintf(fp, L"events\n");
  for (int i = 0; i < static_cast<int>(data.events.size()); ++i) {
    fwprintf(fp, L"  %ls\n", data.events[i].c_str());
  }
  fflush(0);
}
void DisplayDetailedEvents(FILE* fp, const Data& data) {
  assert(fp);
  fwprintf(fp, L"--------------------\n");
  fwprintf(fp, L"event schedule\n");
  sat::Calendar cal_event_from;
  sat::Calendar cal_event_to;
  if ((data.events.size() == 0) || (data.event_is_set == false)) {
    fwprintf(fp, L"Events are not specified or set.\n");
    fflush(0);
    return;
  }
  for (int i = 0; i < static_cast<int>(data.events.size()); ++i) {
    fwprintf(fp, L"  %ls\n", data.events[i].c_str());
    for (int j = 0; j < static_cast<int>(data.jd_event_from[i].size()); ++j) {
      if (data.tz_out == TZ_UT) {
        sat::ToCalendar(data.jd_event_from[i][j], &cal_event_from);
        sat::ToCalendar(data.jd_event_to[i][j], &cal_event_to);
      } else {
        sat::ToCalendar(SATPASS_UT_TO_JST(data.jd_event_from[i][j]),
                        &cal_event_from);
        sat::ToCalendar(SATPASS_UT_TO_JST(data.jd_event_to[i][j]),
                        &cal_event_to);
      }
      fwprintf(fp, L"    %04d/%02d/%02d %02d:%02d:%02d - %04d/%02d/%02d %02d:%02d:%02d (%3s)\n",  // NOLINT
               cal_event_from.year, cal_event_from.mon, cal_event_from.day,
               cal_event_from.hour, cal_event_from.min,
               static_cast<int>(cal_event_from.sec + 0.5),
               cal_event_to.year, cal_event_to.mon, cal_event_to.day,
               cal_event_to.hour, cal_event_to.min,
               static_cast<int>(cal_event_to.sec + 0.5),
               (data.tz_out == TZ_UT) ? L"UT" : L"JST");
    }
  }
  fflush(0);
}
void DisplayPasses(FILE* fp, const Data& data) {
  assert(fp);
  // The console output.
  sat::Calendar cal_aos;
  sat::Calendar cal_mel;
  sat::Calendar cal_los;
  for (int i = 0; i < static_cast<int>(data.nameo.size()); ++i) {
    // The position name is shown.
    fwprintf(fp, L"--------------------\n");
    fwprintf(fp, L"%ls  %3.3f %2.3f %4.1f\n", data.nameo[i].c_str(),
             SAT_TO_DEGREES(data.wo[i].lambda),
             SAT_TO_DEGREES(data.wo[i].phi), data.wo[i].h);
    // The pass list is shown.
    fwprintf(fp, L"\n");
    fwprintf(fp, L"  DATE    %3ls   AZ    MA   %3ls   EL   Az     %3ls   Az   MA   min  event\n", // NOLINT
             (data.tz_out == TZ_UT) ? L"UT" : L"JST",
             (data.tz_out == TZ_UT) ? L"UT" : L"JST",
             (data.tz_out == TZ_UT) ? L"UT" : L"JST");
    int last_day = 0;  // For the day change check.
    for (int j = 0; j < static_cast<int>(data.jd_aos[i].size()); ++j) {
      if (data.tz_out == TZ_UT) {
        sat::ToCalendar(data.jd_aos[i][j], &cal_aos);
        sat::ToCalendar(data.jd_mel[i][j], &cal_mel);
        sat::ToCalendar(data.jd_los[i][j], &cal_los);
      } else {
        sat::ToCalendar(SATPASS_UT_TO_JST(data.jd_aos[i][j]), &cal_aos);
        sat::ToCalendar(SATPASS_UT_TO_JST(data.jd_mel[i][j]), &cal_mel);
        sat::ToCalendar(SATPASS_UT_TO_JST(data.jd_los[i][j]), &cal_los);
      }
      int year = 0;
      if (cal_aos.year < 2057) {
        year = cal_aos.year - 2000;
      } else {
        year = cal_aos.year - 1900;
      }
      if ((j != 0) && (cal_aos.day != last_day)) fwprintf(fp, L"\n");
      fwprintf(fp, L" %02d/%02d/%02d %02d:%02d %05.1f %03d  %02d:%02d %04.1f %05.1f  %02d:%02d %05.1f %03d  %03d  %ls\n",  // NOLINT
               year, cal_aos.mon, cal_aos.day,  // DATE
               cal_aos.hour, cal_aos.min,  // JST or UTC
               data.az_aos[i][j], data.m_aos[i][j],  // AOS
               cal_mel.hour, cal_mel.min,  // JST or UTC
               data.el_mel[i][j], data.az_mel[i][j],  // MEL
               cal_los.hour, cal_los.min,  // JST or UTC
               data.az_los[i][j], data.m_los[i][j],  // LOS
               static_cast<int>(data.duration[i][j]),
               data.event[i][j].c_str());  // Event
      last_day = cal_aos.day;
    }
    fflush(0);
  }
}
bool OutputFile(const Data& data) {
  // The output file.
  FILE* fp = nullptr;
  switch (data.out) {
    case OUTTYPE_TEXT:
#if defined(WIN32) || defined(WIN32GUI)
      _wfopen_s(&fp, L"pass_info.txt", L"w");
#else
      fp = fopen("pass_info.txt", "w");
#endif
      if (fp == nullptr) {
        wprintf(L"Error...Failed to create output file\n");
        return false;
      }
      DisplayTLE(fp, data);
      DisplayDetailedEvents(fp, data);
      DisplayPasses(fp, data);
      fclose(fp);
      wprintf(L"The output pass_info.txt is created\n");
      break;
    case OUTTYPE_HTML:
#if defined(WIN32) || defined(WIN32GUI)
      _wfopen_s(&fp, L"pass_info.html", L"w");
#else
      fp = fopen("pass_info.html", "w");
#endif
      if (fp == nullptr) {
        wprintf(L"Error...Failed to create output file\n");
        return false;
      }
      ExportHTML(data, fp);
      fclose(fp);
      wprintf(L"The output pass_info.html is created\n");
      break;
    default:
      wprintf(L"The output file is not created\n");
      break;
  }
  return true;
}
