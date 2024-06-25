#include "TPPACData.hxx"
#include <iostream>

ClassImp(TPPACData)

uint32_t TPPACData::GetTXSum() { return (tx1 + tx2 - 2*ta); }
uint32_t TPPACData::GetTYSum() { return (ty1 + ty2 - 2*ta); }

void TPPACData::Print() {
    std::cout << " TPPACData" << std::endl;
    std::cout << " X1 = " << tx1 << ", ";
    std::cout << " X2 = " << tx2 << ", ";
    std::cout << " Y1 = " << ty1 << ", ";
    std::cout << " Y2 = " << ty2 << ", ";
    std::cout << " A = " << ta << ", ";
    std::cout << " X = " << x << ", ";
    std::cout << " Y = " << y << "\n";
}