// Default program for dealing with various standard TRIUMF VME setups:
// V792, V1190 (VME), L2249 (CAMAC), Agilent current meter
//
//

#include <stdio.h>
#include <iostream>
#include <time.h>

#include "TRootanaEventLoop.hxx"
#include "TAnaManager.hxx"

#include "TKoBRASniffer.hxx"

class Analyzer : public TRootanaEventLoop
{

public:
  // An analysis manager.  Define and fill histograms in
  // analysis manager.
  TAnaManager *anaManager;
  TKoBRASniffer *sniffer;
  Analyzer()
  {
    // DisableAutoMainWindow();
    UseBatchMode();
    anaManager = 0;
    DisableRootOutput(true);
    SetOnlineName("jsroot_server");
    gettimeofday(&LastUpdateTime, NULL);

    InitManager();
  };

  virtual ~Analyzer() {};

  void TestFunc()
  {
    std::cout << "Test" << std::endl;
  };

  void Initialize()
  {

    std::cout << "Using THttpServer in read/write mode" << std::endl;
    SetTHttpServerReadWrite();

    auto http = GetTHttpServer();
    sniffer = new TKoBRASniffer;
    http->SetSniffer(sniffer);
    // http->RegisterCommand("/DoSomething", "TestFunc()", "button;rootsys/icons/ed_execute.png");
    // http->RegisterCommand("/DoSomething2", "this->TestFunc()", "button;rootsys/icons/ed_interrupt.png");
    // http->SetItemField("/","_toptitle","KoBRA Online Analysis");
  }

  void InitManager()
  {

    if (anaManager)
      delete anaManager;
    anaManager = new TAnaManager();
    anaManager->SetTHttpServer(GetTHttpServer());
  }

  void BeginRun(int transition, int run, int time)
  {
    anaManager->BeginRun(transition, run, time);
    sniffer->SetRunNumber(run);
  }

  void EndRun(int transition, int run, int time)
  {
    anaManager->EndRun(transition, run, time);
  }

  struct timeval LastUpdateTime;

  bool ProcessMidasEvent(TDataContainer &dataContainer)
  {

    if (!anaManager)
      InitManager();

    anaManager->ProcessMidasEvent(dataContainer);

    // Only update the transient histograms (like waveforms or event displays) every second.
    // Otherwise hammers CPU for no reason.
    /*
    struct timeval nowTime;
    gettimeofday(&nowTime, NULL);
    double dtime = nowTime.tv_sec - LastUpdateTime.tv_sec + (nowTime.tv_usec - LastUpdateTime.tv_usec) / 1000000.0;
    if (dtime > 1.0)
    {
      anaManager->UpdateTransientPlots(dataContainer);
      LastUpdateTime = nowTime;
    }
  */
    return true;
  }
};

int main(int argc, char *argv[])
{

  Analyzer::CreateSingleton<Analyzer>();
  return Analyzer::Get().ExecuteLoop(argc, argv);
}
