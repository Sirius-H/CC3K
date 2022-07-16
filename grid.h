#ifndef __GRID_H_
#define __GRID_H_
#include <vector>
#include <iostream>
#include <fstream>
#include "coordinate.h"

class Cell;

class Grid {
    std::vector<std::vector<Cell*>> theGrid;
    std::vector<std::vector<Cell*>> backupGrid;
    Coordinate PCLocation;
public:
    Grid(std::string fileName);
    /*
    void init(char c);
    void updatePlayer();
    void updateGrid();
    bool canMoveTo(Coordinate& cdn);
    void moveTo(Coordinate& oldCdn, Coordinate& newCdn);
    void usePotion(Coordinate& cdn);
    void PCAttack(Coordinate& cdn);
    */
    ~Grid();

};

#endif
