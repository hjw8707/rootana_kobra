#include "TKoBRASCAData.hxx"

#include <iostream>
#include <cstring>


TKoBRASCAData::TKoBRASCAData(int bklen, int bktype, const char* name, void *pdata):
    TGenericData(bklen, bktype, name, pdata)
{
  
  // Do decoding.  Decoding is complicated by the fact that there can be 
  // multiple events in the same bank.  So need to find and save multiple
  // event headers, trailers.
  
  /////////////////////////////////////////////////////////////////////////
  // Data structure: 32 bits (4 bytes)
  //
  // [0]: scaler
  // [1]: scaler2
  // [3]: f3 plastic
  // [5]: all ppac or
  // [7]: HW trig after latch
  // [6]: HW trig before latch
  // [8]: ref pulse
  // [9]: ref time
  // [10]: f2 plastic
  /////////////////////////////////////////////////////////////////////////
  ;
}

void TKoBRASCAData::Print(){

  std::cout << "KoBRASCA decoder for bank " << GetName().c_str() << std::endl;
  std::cout << " Event Time Stamp  : " << GetEventTimeStamp() << std::endl;
  std::cout << " All PPAC OR  Count: " << GetAllPPACORCount() << std::endl;
  std::cout << " F2 Plastic Count  : " << GetF2PlasticCount() << std::endl;
  std::cout << " F3 Plastic Count  : " << GetF3PlasticCount() << std::endl;
  std::cout << " HW Trig. After Latch : " << GetHWTrigAfterLatchCount() << std::endl;
  std::cout << " HW Trig. Before Latch : " << GetHWTrigBeforeLatchCount() << std::endl;
  std::cout << " Get Ref. Pulse : " << GetRefPulse() << std::endl;
  std::cout << " Get Ref. Time: " << GetRefTime() << std::endl;
  

}
