#include "item.h"

Item::Item(Coordinate cdn) : Cell{cdn} {
    canStep = false;
}

std::string Item::getType() {
    return "Item";
}