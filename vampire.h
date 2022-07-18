#ifndef __VAMPIRE_H_
#define __VAMPIRE_H_
#include "npc.h"
#include "subject.h"

class Vampire : public NPC, public Subject {
public: 
    Vampire(Coordinate cdn);
    std::string getName() const override;
};

#endif
