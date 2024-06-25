#include "TKoBRATDCData.hxx"

#include <iostream>
#include <cstring>


TKoBRATDCData::TKoBRATDCData(int bklen, int bktype, const char* name, void *pdata):
    TGenericData(bklen, bktype, name, pdata)
{
  
  // Do decoding.  Decoding is complicated by the fact that there can be 
  // multiple events in the same bank.  So need to find and save multiple
  // event headers, trailers.
  
  /////////////////////////////////////////////////////////////////////////
  // Data structure: 32 bits (4 bytes)
  //
  // [0]: event number
  // [2n-1]: n-th event channel number
  // [2n]: n-th event data
  // ...
  // [last-1]: time stamp
  // [last]: time stamp
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  // Last words are different for the F3PA
  int last_words = 2;
  if (strcmp(name,"F3PA") == 0) last_words = 5;
  /////////////////////////////////////////////////////////////////////////

  // [0]: event number
  fGlobalHeader = GetData32()[0];

  for(int i = 1; i < GetSize() - last_words; i += 2){
    // TDC measurement
    fMeasurements.push_back(KoBRATDCMeasurement(GetData32()[i], GetData32()[i+1]));
  }
  // [last, last-1]: time stamp
  fGlobalTimeStamp = GetData32()[GetSize()-2] | GetData32()[GetSize()-1];
  fLastWord   = GetData32()[GetSize()-1];
  fLastWord_1 = GetData32()[GetSize()-2];

}

void TKoBRATDCData::Print(){

  std::cout << "KoBRATDC decoder for bank " << GetName().c_str() << std::endl;
  std::cout << "event counter = " << GetEventCounter() <<  std::endl;
  for(unsigned int i = 0; i < fMeasurements.size(); i++){
    std::cout << "Measurement: " <<    fMeasurements[i].GetMeasurement() << " for chan "  <<
      fMeasurements[i].GetChannel() << std::endl;
  }
  std::cout << "global time stamp = " << GetGlobalTimeStamp() << std::endl;


}
