#include "kobra_f0.hxx"

#include <fstream>
#include <sstream>

ClassImp(KOBRAF0);

KOBRAF0 *KOBRAF0::instance = nullptr;

KOBRAF0 *KOBRAF0::GetInstance(const char *file) {
    if (instance == nullptr) {
        instance = new KOBRAF0(file);
    }
    return instance;
}

KOBRAF0::KOBRAF0(const char *file) { LoadCSV(file); }

KOBRAF0::~KOBRAF0() {}

// std::vector<std::vector<std::string>> parseCSV(const std::string &filename) {
//     std::ifstream file(filename);
//     std::string line;
//     std::vector<std::vector<std::string>> data;

//     while (std::getline(file, line)) {
//         std::stringstream lineStream(line);
//         std::string cell;
//         std::vector<std::string> row;

//         while (std::getline(lineStream, cell, ',')) {
//             row.push_back(cell);
//         }

//         data.push_back(row);
//     }

//     return data;
// }

void KOBRAF0::LoadCSV(const char *file) {
    std::vector<std::vector<std::string>> data = parseCSV(file);
    data.erase(data.begin());
    for (const auto &row : data) {
        run0.push_back(std::stoi(row[3]));
        run1.push_back(std::stoi(row[4]));
        f0ratio.push_back(std::stod(row[2]));
    }
}

void KOBRAF0::PrintCSV() {
    //////////////////////////////////////////////////////////////
    // Print data
    //////////////////////////////////////////////////////////////
    // Header
    std::cout << "Run0" << " " << "Run1" << " " << "Ratio (%)" << std::endl;
    // Data
    for (size_t i = 0; i < run0.size(); ++i) {
        std::cout << run0[i] << " " << run1[i] << " " << f0ratio[i] << std::endl;
    }
}

double KOBRAF0::GetRatio(int run) {
    // run이 scaler[Run0]와 scaler[Run1]의 사이에 있는 행을 찾고, 해당 행의 Ratio (%)를 반환
    // 그런 것이 없는 경우 0을 반환

    // scaler 맵을 순회하며 Run0, Run1 범위에 해당하는지 확인
    for (size_t i = 0; i < run0.size(); ++i) {
        // Run0, Run1, Ratio (%) 값이 모두 존재하는지 확인
        if (run0[i] <= run && run <= run1[i]) {
            return f0ratio[i];
        }
    }
    return 0;
}
