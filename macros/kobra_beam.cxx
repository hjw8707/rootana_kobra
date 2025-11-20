#include "kobra_beam.hxx"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

ClassImp(KOBRABeam);

KOBRABeam *KOBRABeam::instance = nullptr;

KOBRABeam::KOBRABeam(const char *file) { LoadCSV(file); }

KOBRABeam::~KOBRABeam() {}

void KOBRABeam::LoadCSV(const char *file) {
  floatMap.clear();
  datetimeMap.clear();
  cols.clear();

  std::ifstream fin(file);
  if (!fin.is_open()) {
    std::cerr << "Cannot open file: " << file << std::endl;
    return;
  }

  std::string line;
  // 첫 줄: 컬럼명
  if (std::getline(fin, line)) {
    std::stringstream ss(line);
    std::string col;
    while (std::getline(ss, col, ',')) {
      cols.push_back(col);
    }
  }

  // 데이터 줄
  while (std::getline(fin, line)) {
    std::stringstream ss(line);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, ',')) {
      tokens.push_back(item);
    }
    if (tokens.size() != cols.size()) continue;
    int run = std::stoi(tokens[0]);
    for (size_t i = 1; i < tokens.size(); ++i) {
      // tokens[i]가 실수인지 아닌지 판단해서 실수이면 floatMap에, 아니면 dateTimeMap에 넣기
      try {
        size_t idx;
        std::stod(tokens[i], &idx);
        if (idx == tokens[i].size()) {
          // 전체가 실수로 변환됨
          floatMap[run][cols[i]] = std::stod(tokens[i]);
        } else {
          // 일부만 변환됨(즉, 문자열)
          datetimeMap[run][cols[i]] = tokens[i];
        }
      } catch (const std::invalid_argument &) {
        // 변환 불가, 문자열로 처리
        datetimeMap[run][cols[i]] = tokens[i];
      } catch (const std::out_of_range &) {
        // 너무 큰 값 등, 문자열로 처리
        datetimeMap[run][cols[i]] = tokens[i];
      }
    }
  }
  fin.close();
}

void KOBRABeam::PrintCSV() {
  for (const auto &run_pair : floatMap) {
    std::cout << "Run: " << run_pair.first;
    for (const auto &col_pair : run_pair.second) {
      std::cout << ", " << col_pair.first << ": " << col_pair.second;
    }
    std::cout << std::endl;
  }
  for (const auto &run_pair : datetimeMap) {
    std::cout << "Run: " << run_pair.first;
    for (const auto &col_pair : run_pair.second) {
      std::cout << ", " << col_pair.first << ": " << col_pair.second;
    }
    std::cout << std::endl;
  }
}

double KOBRABeam::GetFloat(int run, const char *col) {
  if (floatMap.count(run) && floatMap[run].count(col)) {
    return floatMap[run][col];
  }
  return 0;
}

std::string KOBRABeam::GetDatetime(int run, const char *col) {
  if (datetimeMap.count(run) && datetimeMap[run].count(col)) {
    return datetimeMap[run][col];
  }
  return "";
}

std::string KOBRABeam::GetStartTime(int run) { return GetDatetime(run, "start_time"); }
std::string KOBRABeam::GetStopTime(int run) { return GetDatetime(run, "stop_time"); }

double KOBRABeam::GetElapsedTime(int run) {
  std::string start_time = GetStartTime(run);
  std::string stop_time = GetStopTime(run);
  // start_time과 stop_time의 시간 차이를 초 단위로 구하는 코드
  // 시간 포맷: "YYYY-MM-DD HH:MM:SS"
  if (start_time.empty() || stop_time.empty()) return 0;

  struct std::tm tm_start = {}, tm_stop = {};
  std::istringstream ss_start(start_time);
  std::istringstream ss_stop(stop_time);

  ss_start >> std::get_time(&tm_start, "%Y-%m-%d %H:%M:%S");
  ss_stop >> std::get_time(&tm_stop, "%Y-%m-%d %H:%M:%S");

  if (ss_start.fail() || ss_stop.fail()) return 0;

  std::time_t t_start = std::mktime(&tm_start);
  std::time_t t_stop = std::mktime(&tm_stop);

  if (t_start == -1 || t_stop == -1) return 0;

  return static_cast<double>(difftime(t_stop, t_start));
}

double KOBRABeam::GetMeanBeamIntensity(int run) { return GetFloat(run, "LEBT_mean"); }

double KOBRABeam::GetIntegralBeamIntensity(int run) { return GetFloat(run, "LEBT_integral"); }

KOBRABeam *KOBRABeam::GetInstance(const char *file) {
  if (!instance) {
    // 파일이 있는지 먼저 체크하고 없으면 instance를 생성하지 않음
    std::ifstream infile(file);
    if (!infile.good()) {
      return nullptr;
    }
    instance = new KOBRABeam(file);
  }
  return instance;
}

// // CSV 파싱 함수 (vector<vector<string>> 반환)
// std::vector<std::vector<std::string>> parseCSV(const std::string &filename) {
//     std::vector<std::vector<std::string>> result;
//     std::ifstream fin(filename);
//     std::string line;
//     while (std::getline(fin, line)) {
//         std::stringstream ss(line);
//         std::string item;
//         std::vector<std::string> tokens;
//         while (std::getline(ss, item, ',')) {
//             tokens.push_back(item);
//         }
//         result.push_back(tokens);
//     }
//     return result;
// }
