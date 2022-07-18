#include "treasure.h"

Treasure::Treasure(Coordinate cdn, int treasure) : Item{cdn}, treasure{treasure} {}

void Treasure::notify(Subject &Dragon) {
    canStep = true;
}

std::string Treasure::getName() const {
    return "Tresure";
}

int Treasure::state() {
    return treasure;
}