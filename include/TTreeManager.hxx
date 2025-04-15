#ifndef TTreeManager_h
#define TTreeManager_h

#include "THeader.hxx"
#include "TTree.h"

class TTreeManager {
   public:
    static TTreeManager* GetInstance();
    inline TTree* GetTree() { return tree; }

    void MakeTree(const char* name, const char* title);
    inline void Fill() { tree->Fill(); }
    inline void Reset() { tree->Reset(); }

    inline void GetLastEntry() { tree->GetEntry(tree->GetEntries() - 1); }

    void MakeHeader();
    inline THeader* GetHeader() { return header; }
    void WriteHeader();

    void LoadAlias(const char* file);

    void SetRunN(int runN) { this->runN = runN; }

   private:
    TTreeManager();
    ~TTreeManager();

    static TTreeManager* instance;
    TTree* tree;

    THeader* header;

    int runN;
};
#endif
