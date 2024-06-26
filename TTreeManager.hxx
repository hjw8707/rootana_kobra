#ifndef TTreeManager_h
#define TTreeManager_h

#include "TTree.h"

class TTreeManager {

public:
static TTreeManager* GetInstance();
inline TTree* GetTree() {return tree;}

void MakeTree(const char* name, const char* title);
inline void Fill() { tree->Fill(); }

private:
TTreeManager();
~TTreeManager();

static TTreeManager* instance;
TTree* tree; 

};
#endif