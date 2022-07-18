#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include "grid.h"
#include "cell.h"
#include "mapelement.h"
#include "wall.h"
#include "floor.h"
#include "passage.h"
#include "stair.h"
#include "textdisplay.h"
#include "pc.h"
#include "human.h"
#include "dwarf.h"
#include "elf.h"
#include "orc.h"

// Debugger
void print( std::vector<Coordinate> const & v ) {
	for ( Coordinate i : v ) std::cout << i << std::endl;
}




Grid::Grid(std::string fileName, char PCName, unsigned seed) {
    // Step 1: create an empty grid of cells, create and connect with TextDisplay
    std::ifstream ifs;
    ifs.open(fileName, std::ios::in);
    std::string s;
    int lineNum = 0;
    std::vector<Coordinate> floors;

    std::vector<std::vector<Cell*>> tempGrid;

    while (std::getline(ifs, s)) { // 目前只实现了floor.txt中仅包含单张地图
        // Debugger
        //std::cout << s << std::endl;
        std::vector<Cell*> tempRow1;
        std::vector<Cell*> tempRow2;
        int len = s.length();
        for (int i = 0; i < len; i++) {
            Coordinate currCdn{lineNum, i};
            Cell* ptr1;
            Cell* ptr2;
            if (s[i] == '|') {
                ptr1 = new Wall{currCdn, 1};
                ptr2 = new Wall{currCdn, 1};
            } if (s[i] == '-') {
                ptr1 = new Wall{currCdn, 2};
                ptr2 = new Wall{currCdn, 1};
            } if (s[i] == ' ') {
                ptr1 = new Wall{currCdn, 3};
                ptr2 = new Wall{currCdn, 1};
            } else if (s[i] == '.') {
                ptr1 = new Floor{currCdn};
                ptr2 = new Floor{currCdn};
                Coordinate currCdn{lineNum, i};
                //allFloors.emplace(currCdn, 0);
                floors.emplace_back(currCdn);

            } else if (s[i] == '#') {
                ptr1 = new Passage{currCdn, 1};
                ptr2 = new Wall{currCdn, 1};
            } else if (s[i] == '+') {
                ptr1 = new Passage{currCdn, 2};
                ptr2 = new Wall{currCdn, 1};
            } else {
                // Debugger
                //std::cout << "this line should not be printed" << std::endl;
            }
            tempRow1.emplace_back(ptr1);
            tempRow2.emplace_back(ptr2);
        }
        lineNum++;
        theGrid.emplace_back(tempRow1);
        tempGrid.emplace_back(tempRow2);
    }
    td = new TextDisplay{theGrid};
    // Debugger
    //std::pair<Coordinate, char> updatedInfo = {Coordinate{1,2}, '@'};
    //setState(updatedInfo);
    //td->notify(*this);

    // Debugger
    //std::cout << *td;


    // split chambers
    // 默认grid不为空
    h = tempGrid.size();
    w = tempGrid[0].size();
    for (int k = 0; k < 5; k++) {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (tempGrid[i][j]->getName() == "Floor") {
                    Coordinate currCdn{i, j};
                    addChamber(tempGrid, currCdn, k);
                }
            }
        }
    }

    int height = tempGrid.size();
    for (int i = 0; i < height; i++) {
        int width = tempGrid[i].size();
        for (int j; j < width; j++) {
            delete tempGrid[i][j];
        }
        tempGrid[i].clear();
    }
    tempGrid.clear();


    
    
    // Step 2: Spawn PC
    std::default_random_engine rng{seed};
    std::shuffle(floors.begin(), floors.end(), rng);
    std::pair<Coordinate, char> newState{floors.at(0), '@'};
    setState(newState);
    td->notify(*this);
    // Debugger
    //std::cout << *td;
    //print(floors);
    int x1 = floors.at(0).x;
    int y1 = floors.at(0).y;
    delete theGrid.at(x1).at(y1);

    if (PCName == 'h') {
        theGrid[x1][y1] = new Human{floors[0]};
        // Debugger
        std::cout << "Human PC created successfully" << std::endl;

    } else if (PCName == 'd') {
        theGrid[x1][y1] = new Dwarf{floors[0]};
        // Debugger
        std::cout << "Dwarf PC created successfully" << std::endl;
    } else if (PCName == 'e') {
        theGrid[x1][y1] = new Elf{floors[0]};
        // Debugger
        std::cout << "Elf PC created successfully" << std::endl;
    } else if (PCName == 'o') {
        theGrid[x1][y1] = new Orc{floors[0]};
        // Debugger
        std::cout << "Orc PC created successfully" << std::endl;
    }
    PCLocation = floors[0];



    // Step 3: Randomly generate Stair
    int x2 = floors.at(1).x;
    int y2 = floors.at(1).y;
    delete theGrid.at(x2).at(y2);
    theGrid[x2][y2] = new Stair{floors[1]};



    // Step 4: NPC random generations




}




Grid::~Grid() {
    int height = theGrid.size();
    for (int i = 0; i < height; i++) {
        int width = theGrid[i].size();
        for (int j; j < width; j++) {
            delete theGrid[i][j];
            //delete backupGrid[i][j];
        }
        theGrid[i].clear();
        //backupGrid[i].clear();
    }
    theGrid.clear();
    //backupGrid.clear();
    //allFloors.clear();
    delete td;
}


void Grid::addChamber(std::vector<std::vector<Cell*>> &tempGrid, Coordinate &c, int i) {
    delete tempGrid[c.x][c.y];
    Coordinate currCdn{c.x, c.y};
    tempGrid[c.x][c.y] = new Wall{c, 1};
    if (i == 1) {
        chamber1.emplace_back(&currCdn);
    } else if (i == 2) {
        chamber2.emplace_back(&currCdn);
    } else if (i == 3) {
        chamber3.emplace_back(&currCdn);
    } else if (i == 4) {
        chamber4.emplace_back(&currCdn);
    } else if (i == 5) {
        chamber5.emplace_back(&currCdn);
    }
    
    if (c.x - 1 >= 0 && tempGrid[c.x - 1][c.y]->getName() == "Floor") {
        Coordinate currCdn{c.x - 1, c.y};
        addChamber(tempGrid, currCdn, i);
    }
    if (c.x + 1 < h && tempGrid[c.x + 1][c.y]->getName() == "Floor") {
        Coordinate currCdn{c.x + 1, c.y};
        addChamber(tempGrid, currCdn, i);
    }
    if (c.y - 1 >= 0 && tempGrid[c.x][c.y - 1]->getName() == "Floor") {
        Coordinate currCdn{c.x, c.y - 1};
        addChamber(tempGrid, currCdn, i);
    }
    if (c.y + 1 < w && tempGrid[c.x][c.y + 1]->getName() == "Floor") {
        Coordinate currCdn{c.x, c.y + 1};
        addChamber(tempGrid, currCdn, i);
    }
    if (c.x - 1 >= 0 && c.y - 1 >= 0 && tempGrid[c.x - 1][c.y - 1]->getName() == "Floor") {
        Coordinate currCdn{c.x - 1, c.y - 1};
        addChamber(tempGrid, currCdn, i);
    }
    if (c.x + 1 < h && c.y - 1 >= 0 && tempGrid[c.x + 1][c.y - 1]->getName() == "Floor") {
        Coordinate currCdn{c.x + 1, c.y - 1};
        addChamber(tempGrid, currCdn, i);
    }
    if (c.x - 1 >= 0 && c.y + 1 < w && tempGrid[c.x - 1][c.y + 1]->getName() == "Floor") {
        Coordinate currCdn{c.x - 1, c.y + 1};
        addChamber(tempGrid, currCdn, i);
    }
    if (c.x + 1 < h && c.y + 1 < w && tempGrid[c.x + 1][c.y + 1]->getName() == "Floor") {
        Coordinate currCdn{c.x + 1, c.y + 1};
        addChamber(tempGrid, currCdn, i);
    }
}


