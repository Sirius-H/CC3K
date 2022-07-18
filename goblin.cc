#include "goblin.h"

Goblin::Goblin(Coordinate cdn) : NPC{cdn} {
    HP = 70;
    Atk = 5;
    Def = 10;
}

std::string Goblin::getName() const {
    return "Goblin";
}
