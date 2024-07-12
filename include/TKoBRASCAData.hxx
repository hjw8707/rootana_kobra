#ifndef TKoBRASCAData_hxx_seen
#define TKoBRASCAData_hxx_seen

#include <vector>

#include "TGenericData.hxx"

class TKoBRASCAData: public TGenericData {

public:

  /// Constructor
  TKoBRASCAData(int bklen, int bktype, const char* name, void *pdata);


  /// Get Event Counter
  uint64_t GetEventTimeStamp() const { return ((uint64_t)GetData32()[1]<<32)|GetData32()[0]; }
  uint32_t GetF2PlasticCount() const { return  GetData32()[10]; }
  uint32_t GetF3PlasticCount() const { return  GetData32()[3]; }
  uint32_t GetAllPPACORCount() const { return  GetData32()[5]; }
  uint32_t GetHWTrigAfterLatchCount() const { return  GetData32()[7]; }
  uint32_t GetHWTrigBeforeLatchCount() const { return  GetData32()[6]; }
  uint32_t GetRefPulse() const { return  GetData32()[8]; }  
  uint32_t GetRefTime() const { return  GetData32()[9]; }  

  void Print();


private:
  
  // We have vectors of the headers/trailers/etc, since there can be 
  // multiple events in a bank.

};

#endif
