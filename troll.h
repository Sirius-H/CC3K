#ifndef __TROLL_H_
#define __TROLL_H_
#include "npc.h"
#include "subject.h"

class Troll : public NPC, public Subject {
public: 
    Troll(Coordinate cdn);
    std::string getName() const override;
};

#endif
