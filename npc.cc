#include <math.h>
#include "npc.h"

NPC::NPC(Coordinate cdn) : Cell{cdn} { }

int NPC::getDefence() const { return Def; }

int NPC::attack(int NPCDefence) const { return ceil(((double)100 / (100 + NPCDefence)) * (Atk + potionAtkEffect)); }

int NPC::attacked(int dmg) { // returns the actual damage to NPC
    int actualDmg = dmg;
    if (HP - actualDmg <= 0) {
        actualDmg = HP;
        HP = 0;
        notifyObserver();
        return actualDmg;
    }
    HP -= actualDmg;
    return actualDmg;
}

std::string NPC::getType() const { return "NPC"; }

int NPC::state() { return moved; }
