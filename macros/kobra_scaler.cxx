#include "kobra_scaler.hxx"

#include <fstream>
#include <sstream>

ClassImp(KOBRAScaler);

KOBRAScaler *KOBRAScaler::instance = nullptr;

KOBRAScaler *KOBRAScaler::GetInstance(const char *file) {
    if (instance == nullptr) {
        instance = new KOBRAScaler(file);
    }
    return instance;
}

KOBRAScaler::KOBRAScaler(const char *file) { LoadCSV(file); }

KOBRAScaler::~KOBRAScaler() {}

std::vector<std::vector<std::string>> parseCSV(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<std::string>> data;

    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    return data;
}

void KOBRAScaler::LoadCSV(const char *file) {
    std::vector<std::vector<std::string>> data = parseCSV(file);
    cols = data[0];
    data.erase(data.begin());
    for (const auto &row : data) {
        int run = std::stoi(row[0]);
        for (size_t i = 1; i < row.size(); ++i) {
            scaler[run][cols[i]] = std::stod(row[i]);
        }
    }
}

void KOBRAScaler::PrintCSV() {
    //////////////////////////////////////////////////////////////
    // Print data
    //////////////////////////////////////////////////////////////
    // Header
    for (const auto &col : cols) {
        std::cout << " " << col;
    }
    std::cout << std::endl;
    // Data
    for (const auto &it : scaler) {
        std::cout << it.first << " ";
        for (const auto &col : cols) {
            std::cout << scaler[it.first][col] << " ";
        }
        std::cout << std::endl;
    }
}

double KOBRAScaler::Get(int run, const char *col) {
    if (scaler.find(run) == scaler.end()) {
        std::cout << "Run " << run << " not found" << std::endl;
        return 0;
    }
    if (scaler[run].find(col) == scaler[run].end()) {
        std::cout << "Column " << col << " not found" << std::endl;
        return 0;
    }
    return scaler[run][col];
}

double KOBRAScaler::GetLiveTime(int run) {
    double hwtriga = Get(run, "hwtriga");
    double hwtrigb = Get(run, "hwtrigb");
    if (hwtriga == 0 || hwtrigb == 0) return 0;
    return hwtriga / hwtrigb;
}

double KOBRAScaler::GetLiveTime(std::vector<int> runs) {
    // hwtriga와 hwtrigb의 합의 비율을 계산
    double sum_hwtriga = 0;
    double sum_hwtrigb = 0;
    for (const auto &run : runs) {
        sum_hwtriga += Get(run, "hwtriga");
        sum_hwtrigb += Get(run, "hwtrigb");
    }
    if (sum_hwtrigb == 0) return 0;
    return sum_hwtriga / sum_hwtrigb;
}

double KOBRAScaler::GetTotalTime(int run) { return Get(run, "ts"); }

double KOBRAScaler::GetTotalTime(std::vector<int> runs) {
    double sum_ts = 0;
    for (const auto &run : runs) {
        sum_ts += Get(run, "ts");
    }
    return sum_ts;
}