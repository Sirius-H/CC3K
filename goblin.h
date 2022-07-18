#ifndef __GOBLIN_H_
#define __GOBLIN_H_
#include "npc.h"
#include "subject.h"

class Goblin : public NPC, public Subject {
public: 
    Goblin(Coordinate cdn);
    std::string getName() const override;
};

#endif
