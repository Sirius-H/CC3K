#include "cell.h"

// Constructor
Cell::Cell(Coordinate cdn): cdn{cdn} { }

bool Cell::canStep() {
    return false;
}

int Cell::attack(int def) {
    return 0;
}

int Cell::attacked(int dmg) {
    return 0;
}

int Cell::getDefence(){
    return 0;
}

// Destructor
Cell::~Cell() { };
