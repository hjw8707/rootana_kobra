// Default program for dealing with various standard TRIUMF VME setups:
// V792, V1190 (VME), L2249 (CAMAC), Agilent current meter
//
//

#include <stdio.h>
#include <iostream>
#include <time.h>

#include "TRootanaEventLoop.hxx"
#include "TAnaManager.hxx"
#include "TTreeManager.hxx"

#include "TTree.h"
class Analyzer : public TRootanaEventLoop
{

public:
  // An analysis manager.  Define and fill histograms in
  // analysis manager.
  TAnaManager *anaManager;
  TTreeManager *treeManager;

  Analyzer()
  {
    // DisableAutoMainWindow();
    UseBatchMode();
    anaManager = 0;
  };

  virtual ~Analyzer() {};

  void Initialize()
  {
#ifdef HAVE_THTTP_SERVER
    std::cout << "Using THttpServer in read/write mode" << std::endl;
    SetTHttpServerReadWrite();
#endif
  }

  void InitManager()
  {
    if (anaManager)
      delete anaManager;
    anaManager = new TAnaManager();
  }

  void BeginRun(int transition, int run, int time)
  {
    InitManager();
    // should come first.
    anaManager->BeginRun(transition, run, time);
  }

  void EndRun(int transition, int run, int time)
  {
    anaManager->EndRun(transition, run, time);
  }

  bool ProcessMidasEvent(TDataContainer &dataContainer)
  {

    if (!anaManager)
      InitManager();

    anaManager->ProcessMidasEvent(dataContainer);
    // treeManager->Fill();

    return true;
  }
};

int main(int argc, char *argv[])
{
  Analyzer::CreateSingleton<Analyzer>();
  return Analyzer::Get().ExecuteLoop(argc, argv);
}
