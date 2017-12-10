  // @file common.cc
  // @brief The common data
  // @author Mamoru Kaminaga
  // @date 2017-09-05 19:51:09
  // Copyright 2017 Mamoru Kaminaga
#ifndef COMMON_H_
#define COMMON_H_
#include <wchar.h>
#include <sat/v1.0.2/sat.h>
#include <sat/v1.0.2/util.h>
#include <tle/v2.1.0/tle.h>
#include <string>
#include <vector>
#define SATPASS_COPYRIGHT         L"Copyright 2017 Mamoru Kaminaga"
#define SATPASS_MAX_DAYS          (21)  // 3 weeks
#define SATPASS_DELTA_SEC         (1)
#define SATPASS_UT_TO_JST(n)      ((n) + 9.0 / 24.0)
#define SATPASS_JST_TO_UT(n)      ((n) - 9.0 / 24.0)
#define SATPASS_TLE_FILE          L"TLE.txt"
#define SATPASS_POS_FILE          L"POS.txt"
#define SATPASS_EVENT_FILE        L"EVENT.txt"
#define TRUE                      1
#define FALSE                     0

enum TZ {
  TZ_UT = 0,
  TZ_JST,
};
enum OUTTYPE {
  OUTTYPE_TEXT = 0,
  OUTTYPE_HTML,
  OUTTYPE_NONE,
};
struct Data {
  TZ tz_in;  // The input time zone
  TZ tz_out;  // The output time zone
  OUTTYPE out;  // The output format
  double jd_start;  // The start Julian day in UT
  double jd_stop;  // The stop Julian day in UT
  sat::TLEDesc tle_desc;  // The TLE description data
  sat::TLEData tle;  // The TLE data
  sat::SatData sat;  // The Satellite data
  std::vector<int> use_event;
  std::vector<std::vector<double>> jd_event_from;  // The event span in jd
  std::vector<std::vector<double>> jd_event_to;  // The event span in jd
  std::vector<std::basic_string<wchar_t>> events;  // The event for the sat
  std::vector<std::basic_string<wchar_t>> nameo;  // Observation point names
  std::vector<sat::WGS84Point> wo;  // Positions of the observer in the WGS-84
  std::vector<double> jd;  // Julian days
  std::vector<double> m;  // The mean anomaly (rad)
  std::vector<sat::Point3> ps;  // Positions of the satellite in the ECEF
  std::vector<sat::WiewedPoint> vp;  // Positions of the satellite in WGS-84
  // The observed data for each observation position
  std::vector<std::vector<int>> m_aos;  // The mean anomaly of AOS (0-255)
  std::vector<std::vector<int>> m_los;  // The mean anomaly of LOS (0-255)
  std::vector<std::vector<double>> jd_aos;  // The Julian day of AOS
  std::vector<std::vector<double>> jd_mel;  // The Julian day of MEL
  std::vector<std::vector<double>> jd_los;  // The Julian day of LOS
  std::vector<std::vector<double>> az_aos;  // The azimuth of AOS (deg)
  std::vector<std::vector<double>> az_mel;  // The azimuth of MEL (deg)
  std::vector<std::vector<double>> az_los;  // The azimuth of LOS (deg)
  std::vector<std::vector<double>> el_mel;  // The elevation of MEL (deg)
  std::vector<std::vector<double>> duration;  // The duration of pass (min)
  std::vector<std::vector<std::basic_string<wchar_t>>> event;  // Duration (min)
  bool event_is_set;  // The event data use flag
};
#endif  // COMMON_H_
