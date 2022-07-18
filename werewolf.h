#ifndef __WEREWOLF_H_
#define __WEREWOLF_H_
#include "npc.h"
#include "subject.h"

class Werewolf : public NPC, public Subject {
public: 
    Werewolf(Coordinate cdn);
    std::string getName() const override;
};

#endif
