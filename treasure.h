#ifndef __TREASURE_H_
#define __TREASURE_H_
#include "item.h"
#include "observer.h"

class Treasure : public Item, public Observer {
    int treasure;

  public:
    int state() override;
    void notify(Subject &Dragon);
    std::string getName() const override;
    Treasure(Coordinate cdn, int treasure);
};

#endif
