#include "vampire.h"

Vampire::Vampire(Coordinate cdn) : NPC{cdn} {
    HP = 50;
    Atk = 25;
    Def = 25;
}

std::string Vampire::getName() const {
    return "Vampire";
}
