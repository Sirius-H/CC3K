#ifndef _COMPASS_H_
#define _COMPASS_H_
#include "item.h"
#include "subject.h"

class Compass : public Item , public Subject {
  public:
    int state() override;
    std::string getName() const override;
    Compass(Coordinate cdn);
};

#endif