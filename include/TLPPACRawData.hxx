#ifndef TLPPACRawData_h
#define TLPPACRawData_h

#include <vector>

#include "TObject.h"

class TLPPACRawData : public TObject {
 public:
  TLPPACRawData() {}
  TLPPACRawData(std::vector<uint32_t> _txl1, std::vector<uint32_t> _txl2, std::vector<uint32_t> _txc1,
                std::vector<uint32_t> _txc2, std::vector<uint32_t> _txr1, std::vector<uint32_t> _txr2,
                std::vector<uint32_t> _ty1, std::vector<uint32_t> _ty2, std::vector<uint32_t> _ta)
      : txl1(_txl1), txl2(_txl2), txc1(_txc1), txc2(_txc2), txr1(_txr1), txr2(_txr2), ty1(_ty1), ty2(_ty2), ta(_ta) {}
  virtual ~TLPPACRawData() {}

  void Clear();
  void AppendTXL1(uint32_t _txl1);
  void AppendTXL2(uint32_t _txl2);
  void AppendTXC1(uint32_t _txc1);
  void AppendTXC2(uint32_t _txc2);
  void AppendTXR1(uint32_t _txr1);
  void AppendTXR2(uint32_t _txr2);
  void AppendTY1(uint32_t _ty1);
  void AppendTY2(uint32_t _ty2);
  void AppendTA(uint32_t _ta);
  void Print();

  std::vector<uint32_t> txl1, txl2, txc1, txc2, txr1, txr2, ty1, ty2, ta;

  ClassDef(TLPPACRawData, 1)
};

#endif
