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
    std::vector<Coordinate> countNeighbour(Coordinate& cdn);
    static int totalCoin;
    Coordinate PCLocation;
    TextDisplay* td;
	unsigned seed;
	double coinVal;	
	int gameDiffLevel;
	// 0: easy mode (more gold, NPC 1/2 HP)
	// 1: medium mode (normal mode)
	// 2: hard mode (less gold, NPC 1.5X Atk) (to be implemented)
    void countNeighbour(Coordinate& cdn, std::vector<Coordinate>& v);
public:
    Grid(std::string fileName, unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(), char PCName = 'h', bool barrierSuit = false, int coinVal = 0);
    void init(char c);
    Coordinate& getPCLocation();
    void updatePlayer();
    void updateGrid();
    bool canMoveTo(Coordinate cdn);
    bool canMoveToNPC(Coordinate cdn);
    bool moveTo(Coordinate newCdn);
    void usePotion(Coordinate cdn);
    void PCAttack(Coordinate cdn);
    void printState();
    ~Grid();
    double getCoin() const;
};

#endif
