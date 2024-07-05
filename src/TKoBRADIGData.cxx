#include "TKoBRADIGData.hxx"

#include <iostream>
#include <cstring>


TKoBRADIGData::TKoBRADIGData(int bklen, int bktype, const char* name, void *pdata):
    TGenericData(bklen, bktype, name, pdata)
{
  
  // Do decoding.  Decoding is complicated by the fact that there can be 
  // multiple events in the same bank.  So need to find and save multiple
  // event headers, trailers.
  
  /////////////////////////////////////////////////////////////////////////
  // Data structure: 32 bits (4 bytes)
  //
  // [0]: channel, [1]: coarse time, [2]: fine time, [3]: ext time,
  // [4]: energy, [5]: energys
  // ...
  /////////////////////////////////////////////////////////////////////////
  for(int i = 0; i < GetSize() ; i += 6){              
    // DIG measurement
    fMeasurements.push_back(KoBRADIGMeasurement(GetData32()[i], GetData32()[i+1],
    GetData32()[i+2], GetData32()[i+3],GetData32()[i+4], GetData32()[i+5]));
  }
}

void TKoBRADIGData::Print(){

  std::cout << "KoBRADIG decoder for bank " << GetName().c_str() << std::endl;
  for(unsigned int i = 0; i < fMeasurements.size(); i++){
    std::cout << "Measurement: " <<    
    fMeasurements[i].GetEnergy() << " for chan "  <<
      fMeasurements[i].GetChannel() << std::endl;
  }

}
