#include "TSSDData.hxx"
#include <iostream>

ClassImp(TSSDData)


void TSSDData::Print() {
    std::cout << " TSSDData" << std::endl;
    std::cout << " CH = " << ch << ", ";
    std::cout << " ADC = " << adc << ", ";
    std::cout << " ACAL  = " << acal << "\n";
}