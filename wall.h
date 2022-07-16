#ifndef __WALL_H__
#define __WALL_H__
#include "mapelement.h"

class Wall: public MapElement {
public:
    Wall(Coordinate cdn);
    std::string getName() override;
};


#endif
