#include "barriersuit.h"

BarrierSuit::BarrierSuit(Coordinate cdn) : Item{cdn} {}

int BarrierSuit::state() { return 10; }

void BarrierSuit::notify(Subject &Dragon) { canStep = true; }

std::string BarrierSuit::getName() const { return "BarrierSuit"; }
