#include "midasio.h"
#include "TMidasEvent.h"
#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char** argv) {

  if (argc < 2) {
    return 0; }

  cout << argv[1] << endl;
  TMReaderInterface* reader = TMNewReader(argv[1]);

  if (reader->fError) {
    printf("Cannot open input file \"%s\"\n",argv[1]);
    delete reader;
    return -1;
  }

  time_t startTimeStamp, stopTimeStamp;
  while (1)
    {
      TMidasEvent event;
      if (!TMReadEvent(reader, &event))
	break;
      
      /// Treat the begin run and end run events differently.
      int eventId = event.GetEventId();

      if ((eventId & 0xFFFF) == 0x8000){// begin run event
        startTimeStamp = event.GetTimeStamp();
      } else if ((eventId & 0xFFFF) == 0x8001){// end run event
	stopTimeStamp = event.GetTimeStamp();
      }
    }
  
  reader->Close();
  delete reader;
  reader = NULL;

  std::cout << "Start Time    : " << ctime(&startTimeStamp);
  std::cout << "Stop Time     : " << ctime(&stopTimeStamp);
  std::cout << "Elapsed second: " << difftime(stopTimeStamp, startTimeStamp) << " sec" << std::endl;
  return 0;}
