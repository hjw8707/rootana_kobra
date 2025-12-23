#ifndef TPPACRawData_h
#define TPPACRawData_h

#include <vector>

#include "TObject.h"

class TPPACRawData : public TObject {
 public:
  TPPACRawData() {}
  TPPACRawData(std::vector<uint32_t> _tx1, std::vector<uint32_t> _tx2, std::vector<uint32_t> _ty1,
               std::vector<uint32_t> _ty2, std::vector<uint32_t> _ta)
      : tx1(_tx1), tx2(_tx2), ty1(_ty1), ty2(_ty2), ta(_ta) {}
  virtual ~TPPACRawData() {}

  void Clear();
  void AppendTX1(uint32_t _tx1);
  void AppendTX2(uint32_t _tx2);
  void AppendTY1(uint32_t _ty1);
  void AppendTY2(uint32_t _ty2);
  void AppendTA(uint32_t _ta);
  void Print();

  std::vector<uint32_t> tx1, tx2, ty1, ty2, ta;

  ClassDef(TPPACRawData, 1)
};

#endif
