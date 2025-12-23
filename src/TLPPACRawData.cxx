#include "TLPPACRawData.hxx"

#include <iostream>

ClassImp(TLPPACRawData);

void TLPPACRawData::Clear() {
  txl1.clear();
  txl2.clear();
  txc1.clear();
  txc2.clear();
  txr1.clear();
  txr2.clear();
  ty1.clear();
  ty2.clear();
  ta.clear();
}

void TLPPACRawData::AppendTXL1(uint32_t _txl1) { txl1.push_back(_txl1); }

void TLPPACRawData::AppendTXL2(uint32_t _txl2) { txl2.push_back(_txl2); }

void TLPPACRawData::AppendTXC1(uint32_t _txc1) { txc1.push_back(_txc1); }

void TLPPACRawData::AppendTXC2(uint32_t _txc2) { txc2.push_back(_txc2); }

void TLPPACRawData::AppendTXR1(uint32_t _txr1) { txr1.push_back(_txr1); }

void TLPPACRawData::AppendTXR2(uint32_t _txr2) { txr2.push_back(_txr2); }

void TLPPACRawData::AppendTY1(uint32_t _ty1) { ty1.push_back(_ty1); }

void TLPPACRawData::AppendTY2(uint32_t _ty2) { ty2.push_back(_ty2); }

void TLPPACRawData::AppendTA(uint32_t _ta) { ta.push_back(_ta); }

void TLPPACRawData::Print() {
  std::cout << " TLPPACRawData" << std::endl;
  std::cout << " TXL1: ";
  for (size_t i = 0; i < txl1.size(); i++) {
    std::cout << txl1[i];
    if (i < txl1.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << " TXL2: ";
  for (size_t i = 0; i < txl2.size(); i++) {
    std::cout << txl2[i];
    if (i < txl2.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << " TXC1: ";
  for (size_t i = 0; i < txc1.size(); i++) {
    std::cout << txc1[i];
    if (i < txc1.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << " TXC2: ";
  for (size_t i = 0; i < txc2.size(); i++) {
    std::cout << txc2[i];
    if (i < txc2.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << " TXR1: ";
  for (size_t i = 0; i < txr1.size(); i++) {
    std::cout << txr1[i];
    if (i < txr1.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << " TXR2: ";
  for (size_t i = 0; i < txr2.size(); i++) {
    std::cout << txr2[i];
    if (i < txr2.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << " TY1: ";
  for (size_t i = 0; i < ty1.size(); i++) {
    std::cout << ty1[i];
    if (i < ty1.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << " TY2: ";
  for (size_t i = 0; i < ty2.size(); i++) {
    std::cout << ty2[i];
    if (i < ty2.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << " TA: ";
  for (size_t i = 0; i < ta.size(); i++) {
    std::cout << ta[i];
    if (i < ta.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;
}
