  // @file export.cc
  // @brief The functions to export the html file.
  // @author Mamoru Kaminaga
  // @date 2017-09-11 16:01:03
  // Copyright 2017 Mamoru Kaminaga
#include <wchar.h>
#include <sat/v1.0.2/data.h>
#include <sat/v1.0.2/util.h>
#include "./common.h"
#include "./export.h"
namespace {
static int open_id = 0;
int NextId(int* id) {
  *id += 1;
  return *id;
}
void WriteHead(const Data& data, FILE* fp) {
  fwprintf(fp, L"\
  <head>\n\
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n\
    <meta name=\"description\" content=\"The output of satpass\">\n\
    <meta name=\"keywords\" content=\"satellite,TLE,pass,satpass\">\n\
    <meta name=\"robots\" content=\"noindex,nofollow\">\n\
    <meta http-equiv=\"Content-Style-Type\" content=\"text/css\">\n\
    <meta http-equiv=\"Content-Script-Type\" content=\"text/javascript\">\n\
    <meta http-equiv=\"pragma\" content=\"no-cache\">\n\
    <meta http-equiv=\"cache-control\" content=\"no-cache\">\n\
    <meta name=\"author\" content=\"satpass %ls\">\n\
    <meta name=\"generator\" content=\"satpass\">\n\
    <title>satpass output</title>\n\
  </head>\n\
  ", SATPASS_COPYRIGHT);
}
void WriteHead1(const Data& data, FILE* fp) {
  // The head sentences.
  fwprintf(fp, L"\
  <center>\n\
    <h1>Satellite Pass Information</h1>\n\
    <h3>About</h3>\n\
    This document tells you about the pass of the satellite %ls.<br>\n\
    The two line elements (TLE) used to get the pass is:\n\
    <pre>\n\
    %ls\n\
    %ls\n\
    %ls\n\
    </pre>\n\
  </center>\n\
  ", data.tle_desc.line_0.c_str(), data.tle_desc.line_0.c_str(),
  data.tle_desc.line_1.c_str(), data.tle_desc.line_2.c_str());

  // The content.
  int tle_div_id = NextId(&open_id);
  fwprintf(fp, L"\
  <div onclick=\"obj=document.getElementById('open%d').style;\n\
    obj.display=(obj.display=='none')?'block':'none';\">\n\
    <a style=\"cursor:pointer;\">\n\
      <font color=\"#0000FF\">\n\
        <strong><center>Show details</center></strong>\n\
      </font>\n\
    </a>\n\
  </div>\n\
  ", tle_div_id);
  // An folded table is written.
  fwprintf(fp, L"<div id=\"open%d\" style=\"display:none;clear:both;\">\n",
           tle_div_id);
  fwprintf(fp, L"<table border=\"1\" align=\"center\" cellspacing=\"0\"\
           cellpadding=\"5\" bordercolor=\"#000000\">\n");
  fwprintf(fp, L"<caption><strong>TLE parameters</strong></caption>\n");

  fwprintf(fp, L"\
  <tr>\n\
    <th bgcolor=\"#404040\"><font color=\"#FFFFFF\">parameter</font></th>\n\
    <th bgcolor=\"#404040\"><font color=\"#FFFFFF\">value</font></th>\n\
    <th bgcolor=\"#404040\"><font color=\"#FFFFFF\">description </font></th>\n\
    <th bgcolor=\"#404040\"><font color=\"#FFFFFF\">unit</font></th> \n\
  </tr>\n"\
  );

  fwprintf(fp, L"\
  <colgroup align=\"center\">\n\
    <col width=\"100\" align=\"center\">\n\
    <col width=\"150\" align=\"center\">\n\
    <col width=\"200\" align=\"left\">\n\
    <col width=\"100\" align=\"center\">\n\
  </colgroup>\n\
  ");

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">name</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">%ls</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"left\">The name</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">none</td>\n\
  </tr>\n"\
  , data.tle_desc.line_0.c_str());

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">num</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">%d</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"left\">The catalog number</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">none</td>\n\
  </tr>\n"\
  , data.tle.sat_num);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">class</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">%c</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"left\">The classification</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">none</td>\n\
  </tr>\n"\
  , data.tle.classification);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">id 1</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">%d</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"left\">The international id (1/3)</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">none</td>\n\
  </tr>\n"\
  , data.tle.id_1);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">id 2</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">%d</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"left\">The international id (2/3)</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">none</td>\n\
  </tr>\n"\
  , data.tle.id_2);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">id 3</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">%ls</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"left\">The international id (3/3)</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">none</td>\n\
  </tr>\n"\
  , data.tle.id_3);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">epoch year</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">%d</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"left\">The epoch of the TLE (year)</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">year</td>\n\
  </tr>\n"\
  , data.tle.epoch_year);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">epoch days</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">%.8f</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"left\">The epoch of the TLE (days)</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">day</td>\n\
  </tr>\n"\
  , data.tle.epoch_days);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">ndot</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">%.8f</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"left\">The change ratio of the mean motion</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">rev/day^2</td>\n\
  </tr>\n"\
  , data.tle.ndot);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">nddot</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">%.8f</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"left\">The change ratio of the ndot</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">rev/day^3</td>\n\
  </tr>\n"\
  , data.tle.nddot);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">bstar</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">%.8f</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"left\">The atmospheric drag coefficient</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">none</td>\n\
  </tr>\n"\
  , data.tle.bstar);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">model</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">%d</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"left\">The model used to acquire the TLE</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">none</td>\n\
  </tr>\n"\
  , data.tle.model);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">snum</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">%d</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"left\">The serial number of the TLE</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">none</td>\n\
  </tr>\n"\
  , data.tle.s_num);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">incl</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">%.8f</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"left\">The inclination</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">deg</td>\n\
  </tr>\n"\
  , data.tle.inclo);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">node</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">%.8f</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"left\">The right ascension of ascending node</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">deg</td>\n\
  </tr>\n"\
  , data.tle.nodeo);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">eccn</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">%.8f</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"left\">The eccentricity</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">none</td>\n\
  </tr>\n"\
  , data.tle.ecco);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">argp</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">%.8f</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"left\">The argument of perigee</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">deg</td>\n\
  </tr>\n"\
  , data.tle.argpo);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">mo</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">%.8f</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"left\">The mean anomaly</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">deg</td>\n\
  </tr>\n"\
  , data.tle.mo);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">no</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">%.8lf</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"left\">The mean motion</td>\n\
    <td bgcolor=\"#CCCCCC\" align=\"center\">rev/day</td>\n\
  </tr>\n"\
  , data.tle.no);

  fwprintf(fp, L"\
  <tr>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">rev</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">%d</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"left\">The rotation count</td>\n\
    <td bgcolor=\"#FFFFFF\" align=\"center\">rev</td>\n\
  </tr>\n"\
  , data.tle.rev);

  fwprintf(fp, L"</table>\n");
  fwprintf(fp, L"</div>\n");
}
void WriteHead2(const Data& data, FILE* fp) {
  // The head sentences.
  fwprintf(fp, L"<h3><center>Pass for each location</center></h3>\n");

  // The content.
  int pos_div_id = 0;
  for (int i = 0; i < static_cast<int>(data.nameo.size()); ++i) {
    pos_div_id = NextId(&open_id);
    fwprintf(fp, L"\
    <div onclick=\"obj=document.getElementById('open%d').style;\n\
      obj.display=(obj.display=='none')?'block':'none';\">\n\
      <a style=\"cursor:pointer;\">\n\
        <font color=\"#0000FF\">\n\
          <strong><center>%ls</center></strong>\n\
        </font>\n\
      </a>\n\
    </div>\n\
    ", pos_div_id, data.nameo[i].c_str());

    // The table for longitude and latitude.
    fwprintf(fp, L"<div id=\"open%d\" style=\"display:none;clear:both;\">\n",
             pos_div_id);
    fwprintf(fp, L"<table border=\"1\" align=\"center\" width=\"360\"\
             cellspacing=\"0\" cellpadding=\"5\" bordercolor=\"#000000\">\n");
    fwprintf(fp, L"\
    <caption><strong>Longitude and latitude of the point</strong></caption>\n\
    <tr>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">parameter</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">value</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">unit</font></th>\n\
    </tr>\n\
    ");

    fwprintf(fp, L"\
    <colgroup align=\"center\">\n\
      <col width=\"130\" align=\"center\">\n\
      <col width=\"130\" align=\"center\">\n\
      <col width=\"30\" align=\"center\">\n\
    </colgroup>\n"\
    );

    fwprintf(fp,L"\
    <tr>\n\
      <td bgcolor=\"#CCCCCC\" align=\"center\">longitude</td>\n\
      <td bgcolor=\"#CCCCCC\" align=\"center\">%04.2f</td>\n\
      <td bgcolor=\"#CCCCCC\" align=\"center\">deg</td>\n\
    </tr>\n"\
    , SAT_TO_DEGREES(data.wo[i].lambda));

    fwprintf(fp,L"\
    <tr>\n\
      <td bgcolor=\"#FFFFFF\" align=\"center\">latitude</td>\n\
      <td bgcolor=\"#FFFFFF\" align=\"center\">%03.2f</td>\n\
      <td bgcolor=\"#FFFFFF\" align=\"center\">deg</td>\n\
    </tr>\n"\
    , SAT_TO_DEGREES(data.wo[i].phi));

    fwprintf(fp, L"\
    <tr>\n\
      <td bgcolor=\"#CCCCCC\" align=\"center\">sea level</td>\n\
      <td bgcolor=\"#CCCCCC\" align=\"center\">%05.1f</td>\n\
      <td bgcolor=\"#CCCCCC\" align=\"center\">m</td>\n\
      </tr>\n"\
    , data.wo[i].h);
    fwprintf(fp,L"</table>\n");

    // The table for pass.
    sat::Calendar cal_aos;
    if (data.tz_out == TZ_UT) {
      sat::ToCalendar(data.jd_aos[i][0], &cal_aos);
    } else {
      sat::ToCalendar(SATPASS_UT_TO_JST(data.jd_aos[i][0]), &cal_aos);
    }
    fwprintf(fp, L"\
    <table border=\"1\" align=\"center\" width=\"500\" cellspacing=\"0\"\
      cellpadding=\"5\" bordercolor=\"#000000\">\n\
      <caption><strong>%02d/%02d</strong></caption>\n"\
    , cal_aos.mon, cal_aos.day);
    fwprintf(fp, L"\
    <tr>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">DATE</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">%3ls</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">AZ</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">MA</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">%3ls</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">EL</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">Az</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">%3ls</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">Az</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">MA</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">min</font></th>\n\
      <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">event</font></th>\n\
    </tr>\n"\
    , (data.tz_out == TZ_UT) ? L"UT" : L"JST",
    (data.tz_out == TZ_UT) ? L"UT" : L"JST",
    (data.tz_out == TZ_UT) ? L"UT" : L"JST");

    int cnt = 0;
    int year = 0;
    int last_day = 0;
    std::basic_string<wchar_t> color;
    sat::Calendar cal_mel;
    sat::Calendar cal_los;
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
      if (cal_aos.year < 2057) {
        year = cal_aos.year - 2000;
      } else {
        year = cal_aos.year - 1900;
      }
      // The day change is checked.
      if ((j != 0) && (cal_aos.day != last_day)) {
        fwprintf(fp,L"</table>\n");
        // The day change, new table.
        fwprintf(fp, L"\
        <table border=\"1\" align=\"center\" width=\"500\" cellspacing=\"0\"\
          cellpadding=\"5\" bordercolor=\"#000000\">\n\
          <caption><strong>%02d/%02d</strong></caption>\n"\
        , cal_aos.mon, cal_aos.day);
        fwprintf(fp, L"\
        <tr>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">DATE</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">%3ls</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">AZ</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">MA</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">%3ls</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">EL</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">Az</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">%3ls</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">Az</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">MA</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">min</font></th>\n\
          <th bgcolor=\"#404040\" align=\"center\"><font color=\"#FFFFFF\">event</font></th>\n\
        </tr>\n"\
        , (data.tz_out == TZ_UT) ? L"UT" : L"JST",
        (data.tz_out == TZ_UT) ? L"UT" : L"JST",
        (data.tz_out == TZ_UT) ? L"UT" : L"JST");
      }
      last_day = cal_aos.day;

      // The data is printed.
      color = (cnt & 1) ? L"#CCCCCC" : L"#FFFFFF";
      fwprintf(fp, L"\
      <tr>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%02d/%02d/%02d</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%02d:%02d</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%05.1f</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%03d</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%02d:%02d</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%04.1f</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%05.1f</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%02d:%02d</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%05.1f</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%03d</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%03d</td>\n\
        <td bgcolor=\"%ls\" align=\"center\" width=\"40\">%ls</td>\n\
      </tr>\n"\
      , color.c_str(), year, cal_aos.mon, cal_aos.day,
        color.c_str(), cal_aos.hour, cal_aos.min,
        color.c_str(), data.az_aos[i][j],
        color.c_str(), data.m_aos[i][j],
        color.c_str(), cal_mel.hour, cal_mel.min,
        color.c_str(), data.el_mel[i][j],
        color.c_str(), data.az_mel[i][j],
        color.c_str(), cal_los.hour, cal_los.min,
        color.c_str(), data.az_los[i][j],
        color.c_str(), data.m_los[i][j],
        color.c_str(), static_cast<int>(data.duration[i][j]),
        color.c_str(), data.event[i][j].c_str());
      ++cnt;
    }
    fwprintf(fp,L"</table>\n");
    fwprintf(fp,L"</div>\n");
  }
}
void WriteNotice(const Data& data, FILE* fp) {
  fwprintf(fp,L"<br>\n");
  fwprintf(fp, L"\
  <center>\n\
    This document is generated by satpass.<br>\n\
    <a href=\"https://github.com/mkaminaga/satpass\">https://github.com/mkaminaga/satpass</a>\n\
  </center>\n"\
  );
}
void WriteBody(const Data& data, FILE* fp) {
  fwprintf(fp, L"<body>\n");
  WriteHead1(data, fp);
  WriteHead2(data, fp);
  WriteNotice(data, fp);
  fwprintf(fp, L"</body>\n");
}
}  // namespace
void ExportHTML(const Data& data, FILE* fp) {
  fwprintf(fp, L"<html>\n");
  WriteHead(data, fp);
  WriteBody(data, fp);
  fwprintf(fp, L"</html>\n");
}
