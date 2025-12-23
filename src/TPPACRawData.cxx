#include "TPPACRawData.hxx"

#include <iostream>

ClassImp(TPPACRawData);

void TPPACRawData::Clear() {
  tx1.clear();
  tx2.clear();
  ty1.clear();
  ty2.clear();
  ta.clear();
}

void TPPACRawData::AppendTX1(uint32_t _tx1) { tx1.push_back(_tx1); }

void TPPACRawData::AppendTX2(uint32_t _tx2) { tx2.push_back(_tx2); }

void TPPACRawData::AppendTY1(uint32_t _ty1) { ty1.push_back(_ty1); }

void TPPACRawData::AppendTY2(uint32_t _ty2) { ty2.push_back(_ty2); }

void TPPACRawData::AppendTA(uint32_t _ta) { ta.push_back(_ta); }

void TPPACRawData::Print() {
  std::cout << " TPPACRawData" << std::endl;
  std::cout << " TX1: ";
  for (size_t i = 0; i < tx1.size(); i++) {
    std::cout << tx1[i];
    if (i < tx1.size() - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  std::cout << " TX2: ";
  for (size_t i = 0; i < tx2.size(); i++) {
    std::cout << tx2[i];
    if (i < tx2.size() - 1) std::cout << ", ";
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
