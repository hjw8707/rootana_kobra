#include "TScalerData.hxx"
#include <iostream>

ClassImp(TScalerData)

void TScalerData::Print() {
    std::cout << " TScalerData" << std::endl;
    std::cout << " TimeStamp = " << ts << "\n";
    std::cout << " PPACOR = " << ppac << "\n";
}

