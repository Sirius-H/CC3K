#include "wall.h"

Wall::Wall(Coordinate cdn): MapElement{cdn} { }

std::string Wall::getName() {
    return "Wall";
}

