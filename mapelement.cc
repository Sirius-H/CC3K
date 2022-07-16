#include "mapelement.h"

MapElement::MapElement(Coordinate cdn): Cell{cdn} { }

std::string MapElement::getType() {
    return "MapElement";
}

int MapElement::state() {
    return -1;
}
