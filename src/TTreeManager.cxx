#include "TTreeManager.hxx"

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

TTreeManager *TTreeManager::instance = NULL;

TTreeManager::TTreeManager() : tree(NULL), header(NULL) {}

void TTreeManager::MakeTree(const char *name, const char *title) {
    tree = new TTree(name, title);

    tree->Branch("runN", &runN, "runN/I");
}

void TTreeManager::MakeHeader() { header = new THeader(); }

void TTreeManager::WriteHeader() { header->Write(); }

TTreeManager *TTreeManager::GetInstance() {
    if (!instance) instance = new TTreeManager();
    return instance;
}

void TTreeManager::LoadAlias(const char *file) {
    std::ifstream fin(file);
    if (not fin.is_open()) {
        std::cerr << "File cannot be opened." << std::endl;
        return;
    }

    std::string line;
    std::regex re(R"((\"[^\"]*\")|([+-]?\d+(\.\d+)?([eE][+-]?\d+)?))");  // 문자열 또는 숫자를 추출하기 위한 정규 표현식
    while (std::getline(fin, line)) {
        // #으로 시작하는 주석을 제거
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) line = line.substr(0, commentPos);

        // 공백만 있는 줄은 무시
        if (line.empty() || line.find_first_not_of(' ') == std::string::npos) continue;

        std::vector<std::string> words;
        auto words_begin = std::sregex_iterator(line.begin(), line.end(), re);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string match_str = match.str();
            if (!match_str.empty()) {
                if (match_str.front() == '"' && match_str.back() == '"') {
                    words.push_back(match_str.substr(1, match_str.size() - 2));
                } else {
                    words.push_back(match_str);
                }
            }
        }

        if (words.size() > 1) {
            tree->SetAlias(words[0].c_str(), words[1].c_str());
        } else
            continue;
        std::cout << " Add the alias: ";
        std::cout << words[0] << " = " << words[1] << std::endl;
    }
    fin.close();
}
