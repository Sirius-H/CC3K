#ifndef __PASSAGE_H__
#define __PASSAGE_H__
#include "mapelement.h"

class Passage: public MapElement {
public:
    Passage(Coordinate cdn);
    bool canStep() override;
    std::string getName() override;
};

#endif
