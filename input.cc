  // @file input.cc
  // @brief Input functions.
  // @author Mamoru Kaminaga
  // @date 2017-09-11 14:42:14
  // Copyright 2017 Mamoru Kaminaga
#include <assert.h>
#include <stdio.h>
#include <wchar.h>
#include <sat/v1.0.2/util.h>
#include <string>
#include <vector>
#include "./common.h"
#include "./input.h"
#include "./output.h"
bool GetInput(Data* data) {
  assert(data);
  // The input TZ is asked.
  wprintf(L"\n");
  wprintf(L"Input 1 / 7, TZ for input, JST ('%d') or UT ('%d') ?:\n",
          TZ_JST, TZ_UT);
  wprintf(L">");
  fflush(0);
#if defined(WIN32)
  wscanf_s(L"%d", &data->tz_in);
#else
  wscanf(L"%d", &data->tz_in);
#endif
  wprintf(L"input time zone is set to %3ls\n",
          (data->tz_in == TZ_UT) ? L"UT" : L"JST");

  // The output TZ is asked.
  wprintf(L"\n");
  wprintf(L"Input 2 / 7, TZ for output, JST ('%d') or UT ('%d') ?:\n",
          TZ_JST, TZ_UT);
  wprintf(L">");
  fflush(0);
#if defined(WIN32)
  wscanf_s(L"%d", &data->tz_out);
#else
  wscanf(L"%d", &data->tz_out);
#endif
  wprintf(L"output time zone is set to %3ls\n",
          (data->tz_out == TZ_UT) ? L"UT" : L"JST");

  // The start time is asked.
  wprintf(L"\n");
  wprintf(L"Input 3 / 7, start \"year mon day hour min sec\" in %3ls:\n",
          (data->tz_in == TZ_UT) ? L"UT" : L"JST");
  wprintf(L">");
  fflush(0);
  sat::Calendar cal;
#if defined(WIN32)
  wscanf_s(L"%d %d %d %d %d %lf", &cal.year, &cal.mon, &cal.day, &cal.hour,
#else
  wscanf(L"%d %d %d %d %d %lf", &cal.year, &cal.mon, &cal.day, &cal.hour,
#endif
           &cal.min, &cal.sec);
  if (data->tz_in == TZ_UT) {
    data->jd_start = sat::ToJulianDay(cal);
  } else {
    data->jd_start = SATPASS_JST_TO_UT(sat::ToJulianDay(cal));
  }
  wprintf(L"start time is set to %.4d/%.2d/%.2d %.2d:%.2d:%.6lf %3ls\n",
          cal.year, cal.mon, cal.day, cal.hour, cal.min, cal.sec,
          (data->tz_in == TZ_UT) ? L"UT" : L"JST");

  // The simulation length is asked.
  wprintf(L"\n");
  wprintf(L"Input 4 / 7, input days to run (1-%d):\n", SATPASS_MAX_DAYS);
  wprintf(L">");
  fflush(0);
  double days = 0.0;
#if defined(WIN32)
  wscanf_s(L"%lf", &days);
#else
  wscanf(L"%lf", &days);
#endif
  if (days < 1.0) {
    wprintf(L"days too small, set to 1\n");
    days = 1;
  } else if (days > SATPASS_MAX_DAYS) {
    wprintf(L"days too large, set to %d\n", SATPASS_MAX_DAYS);
    days = SATPASS_MAX_DAYS;
  }
  data->jd_stop = data->jd_start + days;
  wprintf(L"days is set to %02.2lf\n", days);

  // The event time spans are asked.
  int ans = 0;
  wprintf(L"\n");
  wprintf(L"Input 5 / 7, do you set event schedule? Yes ('1') or not ('0'):\n");
  if (data->events.size() == 0) {
    wprintf(L">This setting is skipped because events are not specified\n");
    data->event_is_set = false;
  } else {
    wprintf(L">");
    fflush(0);
#if defined(WIN32)
    wscanf_s(L"%d", &ans);
#else
    wscanf(L"%d", &ans);
#endif
    data->use_event.resize(data->events.size());
    data->jd_event_from.resize(data->events.size());
    data->jd_event_to.resize(data->events.size());
    if (ans == 1) {
      data->event_is_set = true;
      for (int i = 0; i < static_cast<int>(data->events.size()); ++i) {
        // The event is used.
        data->use_event[i] = TRUE;
        // The span for the event is set.
        bool loop_out = false;
        for (int j = 0; !loop_out; ++j) {
          wprintf(L"\n");
          wprintf(L"Input %ls start \"year mon day hour min sec\" in %3ls:\n",
                  data->events[i].c_str(),
                  (data->tz_in == TZ_UT) ? L"UT" : L"JST");
          wprintf(L">");
          fflush(0);
#if defined(WIN32)
          wscanf_s(L"%d %d %d %d %d %lf", &cal.year, &cal.mon, &cal.day,
#else
          wscanf(L"%d %d %d %d %d %lf", &cal.year, &cal.mon, &cal.day,
#endif
                   &cal.hour, &cal.min, &cal.sec);
          if (data->tz_in == TZ_UT) {
            data->jd_event_from[i].push_back(sat::ToJulianDay(cal));
          } else {
            data->jd_event_from[i].push_back(
                SATPASS_JST_TO_UT(sat::ToJulianDay(cal)));
          }
          wprintf(L"%ls start time is set to %.4d/%.2d/%.2d %.2d:%.2d:%.6lf %3ls\n",  // NOLINT
                  data->events[i].c_str(), cal.year, cal.mon, cal.day, cal.hour,
                  cal.min, cal.sec, (data->tz_in == TZ_UT) ? L"UT" : L"JST");
          // The event length is asked.
          wprintf(L"\n");
          wprintf(L"Input event minutes:\n");
          wprintf(L">");
          fflush(0);
          double minutes = 0.0;
#if defined(WIN32)
          wscanf_s(L"%lf", &minutes);
#else
          wscanf(L"%lf", &minutes);
#endif
          days = minutes / (24.0 * 60.0);
          if (days < 0.0) {
            wprintf(L"Error... Negative value is not allowed for event days\n");
            return false;
          }
          data->jd_event_to[i].push_back(data->jd_event_from[i][j] + days);
          wprintf(L"%ls days is set to %02.2lf\n", data->events[i].c_str(), days);
          // The next span is asked.
          wprintf(L"\n");
          wprintf(L"Add other span for %ls? Yes ('1') or not ('0'):\n",
                  data->events[i].c_str());
          wprintf(L">");
          fflush(0);
#if defined(WIN32)
          wscanf_s(L"%d", &ans);
#else
          wscanf(L"%d", &ans);
#endif
          if (ans != 1) loop_out = true;
        }
      }
    } else {
      data->event_is_set = false;
    }
  }

  // The output file check.
  wprintf(L"\n");
  wprintf(L"Input 6 / 7, output file, text ('0'), html ('1') or none ('2'):\n");
  wprintf(L">");
  fflush(0);
#if defined(WIN32)
  wscanf_s(L"%d", &ans);
#else
  wscanf(L"%d", &ans);
#endif
  switch (ans) {
    case 0:
      data->out = OUTTYPE_TEXT;
      wprintf(L"The text output file.\n");
      break;
    case 1:
      data->out = OUTTYPE_HTML;
      wprintf(L"The html output file.\n");
      break;
    default:
      data->out = OUTTYPE_NONE;
      wprintf(L"No output file.\n");
      break;
  }

  // The input is displayed.
  wprintf(L"--------------------\n");
  wprintf(L"epoch \n");
  // epoch in JST.
  sat::ToCalendar(SATPASS_UT_TO_JST(data->jd_start), &cal);
  wprintf(L"  start epoch %4d/%2d/%2d %2d:%2d:%.6f (JST)\n", cal.year, cal.mon,
          cal.day, cal.hour, cal.min, cal.sec);
  sat::ToCalendar(SATPASS_UT_TO_JST(data->jd_stop), &cal);
  wprintf(L"  end   epoch %4d/%2d/%2d %2d:%2d:%.6f (JST)\n", cal.year, cal.mon,
          cal.day, cal.hour, cal.min, cal.sec);

  // epoch in UT.
  sat::ToCalendar(data->jd_start, &cal);
  wprintf(L"  start epoch %4d/%2d/%2d %2d:%2d:%.6f ( UT)\n", cal.year, cal.mon,
          cal.day, cal.hour, cal.min, cal.sec);
  sat::ToCalendar(data->jd_stop, &cal);
  wprintf(L"  end   epoch %4d/%2d/%2d %2d:%2d:%.6f ( UT)\n", cal.year, cal.mon,
          cal.day, cal.hour, cal.min, cal.sec);

  // Event schedule.
  DisplayDetailedEvents(stdout, *data);

  // The final check.
  wprintf(L"\n");
  wprintf(L"Input 7 / 7, proceed? ('1' or '0'):\n");
  wprintf(L">");
  fflush(0);
#if defined(WIN32)
  wscanf_s(L"%d", &ans);
#else
  wscanf(L"%d", &ans);
#endif
  if (ans == 0) {
    wprintf(L"operation is canceled.\n");
    return false;
  }
  return true;
}
