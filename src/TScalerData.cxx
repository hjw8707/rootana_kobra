#include "TScalerData.hxx"

#include <iostream>

ClassImp(TScalerData);

void TScalerData::Print() {
    std::cout << " TScalerData" << std::endl;
    std::cout << " TimeStamp = " << ts << "\n";
    std::cout << " PPACOR = " << ppac << "\n";
}

// for csv file
void TScalerData::PrintSingleLine(std::ostream &os) {
    os << ts << ", " << ppac << ", " << f2pl << ", " << f3pl << ", " << hwtriga << ", " << hwtrigb << ", " << refp
       << ", " << reft;
}
