#ifndef __GRID_H_
#define __GRID_H_
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <chrono>
#include "coordinate.h"
#include "subject.h"

class Cell;
class TextDisplay;

class Grid: public Subject {
    std::vector<std::vector<Cell*>> theGrid;
    std::vector<std::vector<Coordinate>> chambers;
    int h, w;
    void addChamber(std::vector<std::vector<Cell*>> &tempGrid, Coordinate c, std::vector<Coordinate>& tempChamber);

    Coordinate PCLocation;
    TextDisplay* td;
public:
    Grid(std::string fileName, unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(), char PCName = 'h');
    void init(char c);
    Coordinate& getPCLocation() const;
    void updatePlayer();
    void updateGrid();
    bool canMoveTo(Coordinate& cdn);
    bool canMoveToNPC(Coordinate& cdn);
    void moveTo(Coordinate& oldCdn, Coordinate& newCdn);
    void usePotion(Coordinate& cdn);
    void PCAttack(Coordinate& cdn);
    ~Grid();

};

#endif
