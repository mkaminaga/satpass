  // @file solution.cc
  // @brief The functions to solve orbit parameters.
  // @author Mamoru Kaminaga
  // @date 2017-09-11 16:01:03
  // Copyright 2017 Mamoru Kaminaga
#include <assert.h>
#include <wchar.h>
#include <sat/v1.0.2/data.h>
#include <sat/v1.0.2/sat.h>
#include <sat/v1.0.2/util.h>
#include <tle/v2.1.0/tle.h>
#include <cmath>
#include <string>
#include <vector>
#include "./common.h"
#include "./solution.h"
bool Solve(Data* data) {
  assert(data);
  wprintf(L"--------------------\n");
  wprintf(L"Solving orbital problem\n");
  // The orbital problem solver is initialized.
  if (!sat::InitSat(data->tle, &data->sat)) {
    wprintf(L"solver init failed.\n");
    return false;
  }

  // The orbital problem is solved and the ECEF position of the satellite is
  // derived for each jd.
  double days = data->jd_stop - data->jd_start;
  data->jd.reserve(static_cast<size_t>(days * 86400 / SATPASS_DELTA_SEC));
  data->m.reserve(static_cast<size_t>(days * 86400 / SATPASS_DELTA_SEC));
  data->ps.reserve(static_cast<size_t>(days * 86400 / SATPASS_DELTA_SEC));
  for (double jd = data->jd_start; jd <= data->jd_stop;
       jd += SATPASS_DELTA_SEC / 86400.0) {
    if (!sat::GetSatPos(jd, &data->sat)) {
      wprintf(L"The simulation failed at jd = %.8f.", jd);
      return false;
    }
    // The result is stored.
    data->jd.push_back(jd);
    data->m.push_back(data->sat.m);
    data->ps.push_back(data->sat.p);
  }
  wprintf(L"Succeed... The orbital problem solver converged\n");

  // The satellite position is transformed into the horizontal coordinate
  // system for each observation point.
  enum HCSSTATUS {
    HCSSTATUS_HIDDEN,
    HCSSTATUS_VIEWED,
    HCSSTATUS_IGNORE_FIRST_PASS,
  };
  HCSSTATUS status = HCSSTATUS_HIDDEN;
  data->vp.reserve(static_cast<size_t>(days * 86400 / SATPASS_DELTA_SEC));
  wprintf(L"Calculating...Please wait.\n");
  fflush(0);
  for (int i = 0; i < static_cast<int>(data->nameo.size()); ++i) {
    // Buffers are cleared.
    data->vp.clear();
    // The exceptional case is checked.
    sat::WiewedPoint v;
    sat::ViewSat(data->wo[i], data->ps[0], &v);
    if (v.el > 0) {
      wprintf(L"Warning...%ls AOS in jd start, the first pass is ignored.\n",
              data->nameo[i].c_str());
      fflush(0);
      status = HCSSTATUS_IGNORE_FIRST_PASS;
    }
    sat::ViewSat(data->wo[i], data->ps[data->ps.size() - 1], &v);
    if (v.el > 0) {
      wprintf(L"Warning...%ls AOS in jd stop, the last pass is ignored.\n",
              data->nameo[i].c_str());
    }
    // The loop to buffer data->
    for (int j = 0; j < static_cast<int>(data->ps.size()); ++j) {
      sat::ViewSat(data->wo[i], data->ps[j], &v);
      data->vp.push_back(v);
    }

    // The loop to check AOS, MEL and LOS.
    int j_aos_tmp = 0;
    int j_mel_tmp = 0;
    double el_max = 0.0;
    std::vector<int> j_aos;
    std::vector<int> j_mel;
    std::vector<int> j_los;
    for (int j = 0; j < static_cast<int>(data->ps.size()); ++j) {
      switch (status) {
        case HCSSTATUS_HIDDEN:
          // AOS check.
          if (data->vp[j].el >= 0) {
            j_aos_tmp = j;
            el_max = 0.0;
            status = HCSSTATUS_VIEWED;
          }
          break;
        case HCSSTATUS_VIEWED:
          // MEL check.
          if (data->vp[j].el > el_max) {
            el_max = data->vp[j].el;
            j_mel_tmp = j;
          }
          // LOS check.
          if (data->vp[j].el <= 0) {
            j_aos.push_back(j_aos_tmp);
            j_mel.push_back(j_mel_tmp);
            j_los.push_back(j);
            status = HCSSTATUS_HIDDEN;
          }
          break;
        case HCSSTATUS_IGNORE_FIRST_PASS:
          // LOS check.
          if (data->vp[j].el <= 0) {
            status = HCSSTATUS_HIDDEN;
          }
          break;
        default:
          // No implementation.
          break;
      }
    }
    // The loop to store AOS, MEL and LOS data.
    data->m_aos.push_back(std::vector<int>());
    data->m_los.push_back(std::vector<int>());
    data->jd_aos.push_back(std::vector<double>());
    data->jd_mel.push_back(std::vector<double>());
    data->jd_los.push_back(std::vector<double>());
    data->az_aos.push_back(std::vector<double>());
    data->az_mel.push_back(std::vector<double>());
    data->az_los.push_back(std::vector<double>());
    data->el_mel.push_back(std::vector<double>());
    data->duration.push_back(std::vector<double>());
    double ma = 0.0;
    for (int j = 0; j < static_cast<int>(j_aos.size()); ++j) {
      data->jd_aos[i].push_back(data->jd[j_aos[j]]);
      data->jd_mel[i].push_back(data->jd[j_mel[j]]);
      data->jd_los[i].push_back(data->jd[j_los[j]]);
      data->az_aos[i].push_back(SAT_TO_DEGREES(data->vp[j_aos[j]].az));
      data->az_mel[i].push_back(SAT_TO_DEGREES(data->vp[j_mel[j]].az));
      data->az_los[i].push_back(SAT_TO_DEGREES(data->vp[j_los[j]].az));
      data->el_mel[i].push_back(SAT_TO_DEGREES(data->vp[j_mel[j]].el));
      data->duration[i].push_back((data->jd_los[i][j] - data->jd_aos[i][j]) *
          1440);
      // MA is calculated.
      ma = data->m[j_aos[j]];
      ma = (ma - std::floor(ma)) * 255.0 + 0.5;  // 0 to 255, round
      data->m_aos[i].push_back(static_cast<int>(ma));
      ma = data->m[j_los[j]];
      ma = (ma - std::floor(ma)) * 255.0 + 0.5;  // 0 to 255, round
      data->m_los[i].push_back(static_cast<int>(ma));
    }
    // The time span is checked and the event row is written.
    data->event.push_back(std::vector<std::basic_string<wchar_t>>());
    for (int j = 0; j < static_cast<int>(data->jd_aos[i].size()); ++j) {
      data->event[i].push_back(L"");
    }
    // For each events.
    for (int e = 0; e < static_cast<int>(data->events.size()); ++e) {
      // For each event spans.
      for (int k = 0; k < static_cast<int>(data->jd_event_from[e].size());
          ++k) {
        // For each MEL jd of the position.
        for (int j = 0; j < static_cast<int>(data->jd_mel[i].size()); ++j) {
          // If the MEL is in the span of the event, add the event signature
          // to the event raw of the output html file.
          if ((data->jd_event_from[e][k] < data->jd_mel[i][j]) &&
              (data->jd_mel[i][j] < data->jd_event_to[e][k])) {
            if (data->event[i][j].length() != 0) {
              data->event[i][j] += L", ";
              data->event[i][j] += data->events[e];
            } else {
              data->event[i][j] = data->events[e];
            }
          }
        }
      }
    }
  }
  return true;
}
