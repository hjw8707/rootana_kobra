#include "TKoBRAADCData.hxx"

#include <iostream>
#include <cstring>


TKoBRAADCData::TKoBRAADCData(int bklen, int bktype, const char* name, void *pdata):
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
  // Last words
  int last_words = 4;
  /////////////////////////////////////////////////////////////////////////

  for(int i = 0; i < GetSize() - last_words; i += 2){
    // ADC measurement
    fMeasurements.push_back(KoBRAADCMeasurement(GetData32()[i], GetData32()[i+1]));
  }
  // [last, last-1]: time stamp
  fLastWord   = GetData32()[GetSize()-1];
  fLastWord_1 = GetData32()[GetSize()-2];
  fLastWord_2 = GetData32()[GetSize()-3];

}

void TKoBRAADCData::Print(){

  std::cout << "KoBRAADC decoder for bank " << GetName().c_str() << std::endl;
  for(unsigned int i = 0; i < fMeasurements.size(); i++){
    std::cout << "Measurement: " <<    fMeasurements[i].GetMeasurement() << " for chan "  <<
      fMeasurements[i].GetChannel() << std::endl;
  }
  std::cout << "last word - 1 = " << GetLastWord_1() << std::endl;
  std::cout << "last word - 2 = " << GetLastWord_2() << std::endl;


}
