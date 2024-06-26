#include "TTreeManager.hxx"

TTreeManager* TTreeManager::instance = NULL;

TTreeManager::TTreeManager(): tree(NULL) {
}

void TTreeManager::MakeTree(const char* name, const char* title) {
    tree = new TTree(name, title);
}

TTreeManager* TTreeManager::GetInstance() {
    if (!instance) instance = new TTreeManager();
    return instance;
}
