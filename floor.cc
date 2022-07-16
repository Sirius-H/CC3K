#include "floor.h"

Floor::Floor(Coordinate cdn): MapElement{cdn} { }

bool Floor::canStep() { return true; }

std::string Floor::getName() { return "Floor"; }
