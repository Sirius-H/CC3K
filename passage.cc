#include "passage.h"
Passage::Passage(Coordinate cdn): MapElement{cdn} { }

bool Passage::canStep() { return true; }

std::string Passage::getName() { return "Passage"; }
