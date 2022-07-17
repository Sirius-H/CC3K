#ifndef __CELL_H_
#define __CELL_H_
#include <iostream>
#include "coordinate.h"

class Cell {
    Coordinate cdn;
public:
    Cell(Coordinate cdn);
    virtual int state() = 0;
    virtual std::string getType() const = 0;
    virtual std::string getName() const = 0;
    virtual bool canStep();
    virtual int attack(int def);
    virtual int attacked(int dmg);
    virtual int getDefence();
    virtual ~Cell();
};

#endif