#include "dragon.h"

Dragon::Dragon(Coordinate cdn) : NPC{cdn} {
    HP = 150;
    Atk = 20;
    Def = 20;
}
/*
Dragon::Dragon(Coordinate cdn, BarrierSuit* b) : NPC{cdn} {
    HP = 150;
    Atk = 20;
    Def = 20;
    setObserver(b);
}

Dragon::Dragon(Coordinate cdn, Treasure* t) : NPC{cdn} {
    HP = 150;
    Atk = 20;
    Def = 20;
    setObserver(t);
}
*/
std::string Dragon::getName() const {
    return "Dragon";
}
