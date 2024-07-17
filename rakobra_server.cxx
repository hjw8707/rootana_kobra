//
// RootAna for KoBRA (online monitoring server)
//

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <cstdlib>

#include "TRootanaEventLoop.hxx"
#include "TAnaManager.hxx"

#include "THttpServer.h"
#include "TSystem.h"
#include "TKoBRASniffer.hxx"
#include "TStyle.h"

class Analyzer : public TRootanaEventLoop
{

public:
  // An analysis manager.  Define and fill histograms in
  // analysis manager.
  TAnaManager *anaManager;
  TKoBRASniffer *sniffer;
  THttpServer *http;

  Analyzer()
  {
    // DisableAutoMainWindow();
    UseBatchMode();
    anaManager = 0;
    DisableRootOutput(true);
    SetOnlineName("jsroot_server");
    gettimeofday(&LastUpdateTime, NULL);

    http = new THttpServer("http:9091");
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
    http->SetReadOnly(false);

    sniffer = new TKoBRASniffer;
    http->SetSniffer(sniffer);
    http->AddLocation("midas/",Form("%s/resources",std::getenv("MIDASSYS")));
    http->SetDefaultPage("online.htm");
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);
  }

  void InitManager()
  {
    if (anaManager)
      delete anaManager;
    anaManager = new TAnaManager();
    anaManager->SetTHttpServer(http);
  }

  void BeginRun(int transition, int run, int time)
  {
    anaManager->BeginRun(transition, run, time);
    sniffer->SetRunNumber(run);
    sniffer->SetRunStatus(true);
    http->ProcessRequests();
  }

  void EndRun(int transition, int run, int time)
  {
    anaManager->EndRun(transition, run, time);
    sniffer->SetRunStatus(false);
    http->ProcessRequests();
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
