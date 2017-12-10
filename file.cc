  // @file file.cc
  // @brief File io functions.
  // @author Mamoru Kaminaga
  // @date 2017-09-05 19:51:09
  // Copyright 2017 Mamoru Kaminaga
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <sat/v1.0.2/data.h>
#include <sat/v1.0.2/sat.h>
#include <sat/v1.0.2/util.h>
#include <tle/v2.1.0/tle.h>
#include <string>
#include <vector>
#include "./common.h"
#include "./file.h"
#define MAX_COLUMN        (256)
#define CHECK_IGNORE(s)   ((s[0] == L'#') || (s[0] == L'\0'))
namespace {
  enum TLEREADINGSTATUS {
    TLEREADINGSTATUS_NAME,
    TLEREADINGSTATUS_LINE1,
    TLEREADINGSTATUS_LINE2,
  };
  enum POSREADINGSTATUS {
    POSREADINGSTATUS_NAME,
    POSREADINGSTATUS_LONGITUDE,
    POSREADINGSTATUS_LATITUDE,
    POSREADINGSTATUS_HEIGHT,
    POSREADINGSTATUS_END,
  };
  bool GetLine(FILE* fp, wchar_t* buffer) {
    int cursor = 0;
    wchar_t c = L'\0';
    wchar_t c0 = L'\0';
    wchar_t c1 = L'\0';
    wchar_t c2 = L'\0';
    do {
      if (fread(&c0, 1, 1, fp) == 0) {
        buffer[cursor] = L'\0';
        if (cursor == 0) return false;  // The end of file.
        return true;  // The end of last line.
      }
      c0 &= 0x0000ff;
      if ((0x00 <= static_cast<int>(c0)) && (static_cast<int>(c0) <= 0x7f)) {
        // ASCII, read as 1 byte character.
        c = c0;
      } else if ((0xc0 <= static_cast<unsigned int>(c0)) &&
          (static_cast<unsigned int>(c0) <= 0xdf)) {
        // Out of ASCII, read as 2 Bytes character.
        if (fread(&c1, 1, 1, fp) == 0) return false;
        c1 &= 0x0000ff;
        c = (c1 << 8) | c0;
      } else if ((0xe0 <= static_cast<unsigned int>(c0)) &&
          (static_cast<unsigned int>(c0) <= 0xef)) {
        // Out of ASCII, read as 3 Bytes character.
        if (fread(&c1, 1, 1, fp) == 0) return false;
        if (fread(&c2, 1, 1, fp) == 0) return false;
        c1 &= 0x0000ff;
        c2 &= 0x0000ff;
        c = (c0 << 16) | (c1 << 8) | c2;
      } else {
        // format error.
        return false;
      }
      switch (c) {
        case L'\n':
        case L'\r':
          buffer[cursor] = L'\0';
          return true;  // The end of line.
        default:
          buffer[cursor] = c;
          ++cursor;
          break;
      }
    } while (cursor < MAX_COLUMN);
    return true;
  }
  bool SkipBomAndGetLine(FILE* fp, wchar_t* buffer) {
    uint32_t c = 0;
    if (fread(&c, 3, 1, fp) == 0) return false;  // UTF-8 BOM is EFBBBF.
    if (c == 0x00bfbbef) {
      fseek(fp, 3, SEEK_SET);  // BOM is skipped.
    } else {
      fseek(fp, 0, SEEK_SET);  // There is no BOM.
    }
    return GetLine(fp, buffer);
  }
}  // namespace
bool ReadTLEData(const wchar_t* file_name, Data* data) {
  assert(file_name);
  assert(data);
  FILE* fp = nullptr;
#if defined(WIN32) || defined(WIN32GUI)
  _wfopen_s(&fp, file_name, L"rb");
#else
  char file_name_wcs[256] = {0};
  wcstombs(file_name_wcs, file_name, 256);
  fp = fopen(file_name_wcs, "rb");
#endif
  if (fp == nullptr) {
    wprintf(L"Error... TLE file reading failure\n");
    return false;
  }
  wchar_t buffer[MAX_COLUMN] = {0};
  // The first line is acquired.
  if (!SkipBomAndGetLine(fp, buffer)) {
    wprintf(L"Error... TLE file is empty\n");
    fclose(fp);
    return false;
  }
  int column = 0;
  TLEREADINGSTATUS s = TLEREADINGSTATUS_NAME;
  do {
    column = static_cast<int>(wcslen(buffer));
    if (CHECK_IGNORE(buffer)) continue;  // This line is skipped.
    switch (s) {
      case TLEREADINGSTATUS_NAME:
        data->tle_desc.line_0 = buffer;
        s = TLEREADINGSTATUS_LINE1;
        break;
      case TLEREADINGSTATUS_LINE1:
        if (column < 68) {
          wprintf(L"Error... TLE line 1 is too short\n");
          fclose(fp);
          return false;
        }
        data->tle_desc.line_1 = buffer;
        s = TLEREADINGSTATUS_LINE2;
        break;
      case TLEREADINGSTATUS_LINE2:
        if (column < 68) {
          wprintf(L"Error... TLE line 2 is too short\n");
          fclose(fp);
          return false;
        }
        data->tle_desc.line_2 = buffer;
        // The data is added to buffer.
        sat::ReadTLE(data->tle_desc, &data->tle);
        // The file is closed.
        fclose(fp);
        return true;
      default:
        // No implementation.
        break;
    }
  } while (GetLine(fp, buffer));
  fclose(fp);
  return true;
}
bool ReadPositions(const wchar_t* file_name, Data* data) {
  assert(file_name);
  assert(data);
  // The data is cleared.
  data->wo.clear();
  data->nameo.clear();
  FILE* fp = nullptr;
#if defined(WIN32) || defined(WIN32GUI)
  _wfopen_s(&fp, file_name, L"rb");
#else
  char file_name_wcs[256] = {0};
  wcstombs(file_name_wcs, file_name, 256);
  fp = fopen(file_name_wcs, "rb");
#endif
  if (fp == nullptr) {
    wprintf(L"Error... Position file reading failure\n");
    return false;
  }
  wchar_t buffer[MAX_COLUMN] = {0};
  // The first line is acquired.
  if (!SkipBomAndGetLine(fp, buffer)) {
    wprintf(L"Error... Position file is empty\n");
    fclose(fp);
    return false;
  }
  // The BOM is removed.
  POSREADINGSTATUS s = POSREADINGSTATUS_NAME;
  std::basic_string<wchar_t> nameo;
  sat::WGS84Point wo;
  do {
    if (CHECK_IGNORE(buffer)) continue;  // This line is skipped.
    switch (s) {
      case POSREADINGSTATUS_NAME:
        nameo = buffer;
        s = POSREADINGSTATUS_LONGITUDE;
        break;
      case POSREADINGSTATUS_LONGITUDE:
#if defined(WIN32) || defined(WIN32GUI)
        swscanf_s(buffer, L"%lf", &wo.lambda);
#else
        swscanf(buffer, L"%lf", &wo.lambda);
#endif
        wo.lambda = SAT_TO_RADIANS(wo.lambda);
        s = POSREADINGSTATUS_LATITUDE;
        break;
      case POSREADINGSTATUS_LATITUDE:
#if defined(WIN32) || defined(WIN32GUI)
        swscanf_s(buffer, L"%lf", &wo.phi);
#else
        swscanf(buffer, L"%lf", &wo.phi);
#endif
        wo.phi = SAT_TO_RADIANS(wo.phi);
        s = POSREADINGSTATUS_HEIGHT;
        break;
      case POSREADINGSTATUS_HEIGHT:
#if defined(WIN32) || defined(WIN32GUI)
        swscanf_s(buffer, L"%lf", &wo.h);
#else
        swscanf(buffer, L"%lf", &wo.h);
#endif
        s = POSREADINGSTATUS_NAME;
        // The data is added to buffer.
        data->nameo.push_back(nameo);
        data->wo.push_back(wo);
        break;
      case POSREADINGSTATUS_END:
        fclose(fp);
        return true;
      default:
        // No implementation.
        break;
    }
  } while (GetLine(fp, buffer));
  fclose(fp);
  return true;
}
bool ReadEvent(const wchar_t* file_name, Data* data) {
  assert(file_name);
  assert(data);
  // The data is cleared.
  data->events.clear();
  FILE* fp = nullptr;
#if defined(WIN32) || defined(WIN32GUI)
  _wfopen_s(&fp, file_name, L"rb");
#else
  char file_name_wcs[256] = {0};
  wcstombs(file_name_wcs, file_name, 256);
  fp = fopen(file_name_wcs, "rb");
#endif
  if (fp == nullptr) {
    wprintf(L"Error... Event file reading failure\n");
    return false;
  }
  wchar_t buffer[MAX_COLUMN] = {0};
  // The first line is acquired.
  if (!SkipBomAndGetLine(fp, buffer)) {
    wprintf(L"Notice... Events are not specified\n");
    fclose(fp);
    return true;
  }
  // The BOM is removed.
  do {
    if (CHECK_IGNORE(buffer)) continue;  // This line is skipped.
    data->events.push_back(buffer);
  } while (GetLine(fp, buffer));
  fclose(fp);
  return true;
}
