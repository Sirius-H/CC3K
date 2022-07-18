#include "troll.h"

Troll::Troll(Coordinate cdn) : NPC{cdn} {
    HP = 120;
    Atk = 25;
    Def = 15;
}

std::string Troll::getName() const {
    return "Troll";
}
