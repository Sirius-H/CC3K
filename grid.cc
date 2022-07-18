#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include <memory>
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




Grid::Grid(std::string fileName, unsigned seed, char PCName) {
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
            } else if (s[i] == '-') {
                ptr1 = new Wall{currCdn, 2};
                ptr2 = new Wall{currCdn, 2};
            } else if (s[i] == ' ') {
                ptr1 = new Wall{currCdn, 3};
                ptr2 = new Wall{currCdn, 3};
            } else if (s[i] == '.') {
                ptr1 = new Floor{currCdn};
                ptr2 = new Floor{currCdn};
                Coordinate currCdn{lineNum, i};
                //allFloors.emplace(currCdn, 0);
                floors.emplace_back(currCdn);

            } else if (s[i] == '#') {
                ptr1 = new Passage{currCdn, 1};
                ptr2 = new Passage{currCdn, 1};
            } else if (s[i] == '+') {
                ptr1 = new Passage{currCdn, 2};
                ptr2 = new Passage{currCdn, 2};
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
    int chamberIndex = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (tempGrid[i][j]->getName() == "Floor") {
                std::vector<Coordinate> tempChamber;
                addChamber(tempGrid, Coordinate{i,j}, tempChamber);
                chambers.emplace_back(tempChamber);
                chamberIndex++;
            }
        }
    }

    // Debugger section
    /*
    int chamberNum = chambers.size();
    std::cout << ">>>>>>> CHAMBER NUM: " << chamberNum << std::endl;
    for (int i = 0; i < chamberNum; i++) {
        int floorNum = chambers[i].size();
        std::cout << ">>>>>>> Floor NUM: " << floorNum << std::endl;
        for (int j = 0; j < floorNum; j++) {
            std::cout << chambers[i][j] << std::endl;
        }
    }
    */




    int height = tempGrid.size();
    for (int i = 0; i < height; i++) {
        int width = tempGrid[i].size();
        for (int j = 0; j < width; j++) {
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
    floors.clear();



    // Step 4: NPC random generations




}




Grid::~Grid() {
    // Deleting theGrid
    int height = theGrid.size();
    for (int i = 0; i < height; i++) {
        int width = theGrid[i].size();
        for (int j = 0; j < width; j++) {
            delete theGrid[i][j];
        }
        theGrid[i].clear();
    }
    theGrid.clear();
    // Deleting TextDisplay
    delete td;
    // Deleting chambers
    int chamberNum = chambers.size();
    for (int i = 0; i < chamberNum; i++) {
        chambers[i].clear();
    }
    chambers.clear();
}


void Grid::addChamber(std::vector<std::vector<Cell*>> &tempGrid, Coordinate c, std::vector<Coordinate>& tempChamber) {
    delete tempGrid[c.x][c.y];
    //Coordinate* currCdn = new Coordinate{c.x, c.y};
    tempGrid[c.x][c.y] = new Wall{c, 1};
    tempChamber.emplace_back(c);    
    if (c.x - 1 >= 0 && tempGrid[c.x - 1][c.y]->getName() == "Floor") {
        addChamber(tempGrid, Coordinate{c.x - 1, c.y}, tempChamber);
    }
    if (c.x + 1 < h && tempGrid[c.x + 1][c.y]->getName() == "Floor") {
        addChamber(tempGrid, Coordinate{c.x + 1, c.y}, tempChamber);
    }
    if (c.y - 1 >= 0 && tempGrid[c.x][c.y - 1]->getName() == "Floor") {
        addChamber(tempGrid, Coordinate{c.x, c.y - 1}, tempChamber);
    }
    if (c.y + 1 < w && tempGrid[c.x][c.y + 1]->getName() == "Floor") {
        addChamber(tempGrid, Coordinate{c.x, c.y + 1}, tempChamber);
    }
    if (c.x - 1 >= 0 && c.y - 1 >= 0 && tempGrid[c.x - 1][c.y - 1]->getName() == "Floor") {
        addChamber(tempGrid, Coordinate{c.x - 1, c.y - 1}, tempChamber);
    }
    if (c.x + 1 < h && c.y - 1 >= 0 && tempGrid[c.x + 1][c.y - 1]->getName() == "Floor") {
        addChamber(tempGrid, Coordinate{c.x + 1, c.y - 1}, tempChamber);
    }
    if (c.x - 1 >= 0 && c.y + 1 < w && tempGrid[c.x - 1][c.y + 1]->getName() == "Floor") {
        addChamber(tempGrid, Coordinate{c.x - 1, c.y + 1}, tempChamber);
    }
    if (c.x + 1 < h && c.y + 1 < w && tempGrid[c.x + 1][c.y + 1]->getName() == "Floor") {
        addChamber(tempGrid, Coordinate{c.x + 1, c.y + 1}, tempChamber);
    }
}

void Grid::updateGrid() {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (theGrid[i][j]->getType() == "Enemy" && theGrid[i][j]->state() == 0) { // if player is within 1 unit, attack
                if ((PCLocation.x == i || PCLocation.x == i + 1 || PCLocation.x == i - 1) && (PCLocation.y == j || PCLocation.y == j - 1 || PCLocation.y == j + 1)) {
                    int def = theGrid[PCLocation.x][PCLocation.y]->getDefence();
                    int dmg = theGrid[i][j]->attack(def);
                    theGrid[PCLocation.x][PCLocation.y]->attacked(dmg);
                    theGrid[i][j]->setState();
                } else { // else move one block
                    std::vector<Coordinate> v;
                    v.emplace_back(Coordinate{i, j + 1});
                    v.emplace_back(Coordinate{i, j - 1});
                    v.emplace_back(Coordinate{i + 1, j});
                    v.emplace_back(Coordinate{i - 1, j});
                    v.emplace_back(Coordinate{i + 1, j + 1});
                    v.emplace_back(Coordinate{i + 1, j - 1});
                    v.emplace_back(Coordinate{i - 1, j + 1});
                    v.emplace_back(Coordinate{i - 1, j - 1});
                    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                    std::shuffle(v.begin(), v.end(), std::default_random_engine(seed));
                    for (int k = 0; k < 8; k++) {
                        if (canMoveToNPC(v[k]) == true) {
                            delete theGrid[v[k].x][v[k].y];
                            theGrid[v[k].x][v[k].y] = theGrid[i][j];
                            theGrid[i][j] = new Floor{Coordinate{i, j}};
                            theGrid[v[k].x][v[k].y]->setState();
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool Grid::canMoveToNPC(Coordinate& cdn) {
    if (theGrid[cdn.x][cdn.y]->getName() == "Floor") {
        return true;
    }
    return false;
}

bool Grid::canMoveTo(Coordinate& cdn) {
    if (theGrid[cdn.x][cdn.y]->canStep() == true) {
        return true;
    } else if (theGrid[cdn.x][cdn.y]->getName() == "Wall") {
        throw "You should not be moving on to a wall";
    } else if (theGrid[cdn.x][cdn.y]->getName() == "Potion") {
        throw "You should not be moving on to a potion";
    } else if (theGrid[cdn.x][cdn.y]->getType() == "NPC") {
        throw "You should not be moving on to a NPC";
    } else if (theGrid[cdn.x][cdn.y]->getName() == "Treasure") {
        throw "You need to beat the dragon to unlock this item";
    } else if (theGrid[cdn.x][cdn.y]->getName() == "BarrierSuit") {
        throw "You need to beat the dragon to unlock this item";
    }
    return false;
}

bool Grid::moveTo(Coordinate& oldCdn, Coordinate& newCdn) {
    if (theGrid[newCdn.x][newCdn.y]->getName() == "Stair") {
        return true;
    }
    if (canMoveTo(newCdn) == true) {
        if (theGrid[newCdn.x][newCdn.y]->getType() == "Item") {
            int code = theGrid[newCdn.x][newCdn.y]->state();
            PC* p = dynamic_cast<PC*>(theGrid[PCLocation.x][PCLocation.y]);
            p->applyEffect(code);
        }
        delete theGrid[newCdn.x][newCdn.y];
        theGrid[newCdn.x][newCdn.y] = theGrid[PCLocation.x][PCLocation.y];
        theGrid[PCLocation.x][PCLocation.y] = new Floor{PCLocation};
        PCLocation = newCdn;
    }
}


