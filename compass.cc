#include "compass.h"

Compass::Compass(Coordinate cdn) : Item{cdn} { }

int Compass::state() { return 11; }

std::string Compass::getName() const { return "Compass"; }
