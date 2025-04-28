#include "TLPPACData.hxx"

#include <iostream>

ClassImp(TLPPACData);

uint32_t TLPPACData::GetTXLSum() { return (txl1 + txl2 - 2 * ta); }
uint32_t TLPPACData::GetTXCSum() { return (txc1 + txc2 - 2 * ta); }
uint32_t TLPPACData::GetTXRSum() { return (txr1 + txr2 - 2 * ta); }
uint32_t TLPPACData::GetTYSum() { return (ty1 + ty2 - 2 * ta); }

void TLPPACData::Print() {
    std::cout << " TLPPACData" << std::endl;
    std::cout << " XL1 = " << txl1 << ", ";
    std::cout << " XL2 = " << txl2 << ", ";
    std::cout << " XC1 = " << txc1 << ", ";
    std::cout << " XC2 = " << txc2 << ", ";
    std::cout << " XR1 = " << txr1 << ", ";
    std::cout << " XR2 = " << txr2 << ", ";
    std::cout << " Y1 = " << ty1 << ", ";
    std::cout << " Y2 = " << ty2 << ", ";
    std::cout << " A = " << ta << ", ";
    std::cout << " X = " << x << ", ";
    std::cout << " Y = " << y << ",";
    std::cout << " TXSum = " << txsum << ", ";
    std::cout << " TYSum = " << tysum << "\n";
}