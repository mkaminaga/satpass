  // @file common.cc
  // @brief The common data.
  // @author Mamoru Kaminaga
  // @date 2017-12-07 12:30:11
  // Copyright 2017 Mamoru Kaminaga
#include <wchar.h>
#include <windows.h>
#include "./common.h"

double ToTimeLocal(double jd_ut, const Data& data) {
  return jd_ut + data.time_locale_delta_hour / 24.0;
}
double ToTimeUT(double jd_local, const Data& data) {
  return jd_ut - data.time_locale_delta_hour / 24.0;
}
