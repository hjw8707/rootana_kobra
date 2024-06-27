#include "TPlasticData.hxx"
#include <iostream>

ClassImp(TPlasticData)


void TPlasticData::Print() {
    std::cout << " TPlasticData" << std::endl;
    std::cout << " TL = " << tl << ", ";
    std::cout << " TR = " << tr << ", ";
    std::cout << " TSum  = " << tsum << ", ";
    std::cout << " TDiff = " << tdiff << ",";
    std::cout << " AL = " << al << ", ";
    std::cout << " AR = " << ar << "\n";    
}