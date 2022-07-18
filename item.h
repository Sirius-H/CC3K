#ifndef __ITEM_H__
#define __ITEM_H__
#include "cell.h"

class Item : public Cell {
protected:
  bool canStep;
public:
  std::string getType() const override;
  Item(Coordinate cdn);
};

#endif
