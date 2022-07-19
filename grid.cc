#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include "termcodes.h"
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
#include "potion.h"
#include "treasure.h"
#include "barriersuit.h"
#include "npc.h"
#include "merchant.h"

// Debugger
void print( std::vector<Coordinate> const & v ) {
	for ( Coordinate i : v ) std::cout << i << std::endl;
}




Grid::Grid(std::string fileName, unsigned seed, char PCName, bool barrierSuit): seed{seed} {
    // Step 1: create an empty grid of cells, create and connect with TextDisplay
    std::ifstream ifs;
    ifs.open(fileName, std::ios::in);
    std::string s;
    int lineNum = 0;

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
    std::vector<int> num;
    int chamber_size = chambers.size();
    for (int i = 0; i < chamber_size; i++) {
        num.emplace_back(i);
    }
    std::shuffle(num.begin(), num.end(), rng);
    std::vector<Coordinate> PCchamber = chambers[num[0]];
    std::shuffle(PCchamber.begin(), PCchamber.end(), rng);
    std::pair<Coordinate, char> newState{PCchamber.at(0), '@'};
    setState(newState);
    td->notify(*this); 
    // Debugger
    //std::cout << *td;
    //print(floors);
    int x1 = PCchamber.at(0).x;
    int y1 = PCchamber.at(0).y;
    delete theGrid.at(x1).at(y1);

    if (PCName == 'h') {
        theGrid[x1][y1] = new Human{PCchamber[0]};
        // Debugger
        std::cout << "Human PC created successfully" << std::endl;

    } else if (PCName == 'd') {
        theGrid[x1][y1] = new Dwarf{PCchamber[0]};
        // Debugger
        std::cout << "Dwarf PC created successfully" << std::endl;
    } else if (PCName == 'e') {
        theGrid[x1][y1] = new Elf{PCchamber[0]};
        // Debugger
        std::cout << "Elf PC created successfully" << std::endl;
    } else if (PCName == 'o') {
        theGrid[x1][y1] = new Orc{PCchamber[0]};
        // Debugger
        std::cout << "Orc PC created successfully" << std::endl;
    }
    PCLocation = PCchamber[0];



    // Step 3: Randomly generate Stair
	std::shuffle(num.begin(), num.end(), std::default_random_engine{seed + 1});
    std::vector<Coordinate> stairChamber = chambers[num[0]];
    std::shuffle(stairChamber.begin(), stairChamber.end(), std::default_random_engine{seed + 1}));
    int x2 = stairChamber.at(0).x;
    int y2 = stairChamber.at(0).y;
    delete theGrid.at(x2).at(y2);
    theGrid[x2][y2] = new Stair{stairChamber[0]};
    



    // Step 4: potion generations
    for (int i = 0; i < 10; i++) {
		std::shuffle(num.begin(), num.end(), std::default_random_engine{seed + 2 + i}));
        std::vector<Coordinate> potionChamber = chambers[num[0]];
        std::shuffle(potionChamber.begin(), potionChamber.end(), std::default_random_engine(seed + 2 + i));
		for (size_t i = 0; i < potionChamber.size(); i++) {
			if (canMoveTo(potionChamber[i])) {
	            int x3 = potionChamber[i].x;
	            int y3 = potionChamber[i].y;
	            delete theGrid[x3][y3];
	            theGrid[x3][y3] = new Potion{potionChamber[i], randomInt(6, seed + 3 + i)};
				break;
			}
		}
        potionChamber.clear();
    }

    // Step 5: Gold
    for (int i = 0; i < 10; i++) {
        unsigned temp_seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(num.begin(), num.end(), std::default_random_engine(temp_seed));
        std::vector<Coordinate> goldChamber = chambers[num[0]];
        std::shuffle(goldChamber.begin(), goldChamber.end(), std::default_random_engine(temp_seed));
        if (canMoveTo(goldChamber[0])) {
            int x3 = goldChamber[0].x;
            int y3 = goldChamber[0].y;
            delete theGrid[x3][y3];
            int ri = randomInt(8);
            int treasure = 0;
            if (ri <= 4) {
                treasure = 6;
            } else if (ri == 5) {
                treasure = 9;
            } else if (ri > 5) {
                treasure = 7;
            }
            theGrid[x3][y3] = new Treasure{goldChamber[0], treasure};
        } else {
            i--;
        }
        goldChamber.clear();
    }

    if (barrierSuit) {
        while (true) {
            unsigned temp_seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(num.begin(), num.end(), std::default_random_engine(temp_seed));
            std::vector<Coordinate> bsChamber = chambers[num[0]];
            std::shuffle(bsChamber.begin(), bsChamber.end(), std::default_random_engine(temp_seed));
            int x4 = bsChamber[0].x;
            int y4 = bsChamber[0].y;
            if (theGrid[x4][y4]->getName() == "Floor") {
                delete theGrid[x4][y4];
                theGrid[x4][y4] = new BarrierSuit{bsChamber[0]};
                bsChamber.clear();
                break;
            }
            bsChamber.clear();
        }
    }

    // Step 6: NPC generation
    



    stairChamber.clear();
    PCchamber.clear();
    num.clear();
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

int Grid::randomInt(int x, unsigned seed) { 
    std::vector<int> num;
    for (int i = 0; i < x; i++) {
        num.emplace_back(i);
    }
    std::shuffle(num.begin(), num.end(), std::default_random_engine(seed));
    int answer = num[0];
    num.clear();
    return answer;
}

void Grid::updateGrid() {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (theGrid[i][j]->getType() == "NPC" && theGrid[i][j]->state() == 0) {
                NPC* n = dynamic_cast<NPC*>(theGrid[i][j]);
                 // if player is within 1 unit, attack
                if ((PCLocation.x == i || PCLocation.x == i + 1 || PCLocation.x == i - 1) && (PCLocation.y == j || PCLocation.y == j - 1 || PCLocation.y == j + 1)) {
                    int def = theGrid[PCLocation.x][PCLocation.y]->getDefence();
                    int dmg = theGrid[i][j]->attack(def);
                    theGrid[PCLocation.x][PCLocation.y]->attacked(dmg);
                    n->setState();
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
                            n->setState();
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool Grid::canMoveToNPC(Coordinate& cdn) {
    std::string name = theGrid[cdn.x][cdn.y]->getName();
    if (name == "Floor") {
        return true;
    }
    if (name == "Stair") {
        return !theGrid[cdn.x][cdn.y]->state(); // 如果目前玩家已经拿到了compass (i.e.: stair revealed = true), 则NPC不可以踩在stair上；如果not revealed，就可以当成普通floor随便踩
    }
    return false;
}

bool Grid::canMoveTo(Coordinate& cdn) { // for PC
    if (theGrid[cdn.x][cdn.y]->canStep() == true) {
        return true;
    } else if (theGrid[cdn.x][cdn.y]->getName() == "Wall") {
        std::cout << "You should not be moving on to a wall" << std::endl;
    } else if (theGrid[cdn.x][cdn.y]->getName() == "Potion") {
        std::cout << "You should not be moving on to a potion" << std::endl;
    } else if (theGrid[cdn.x][cdn.y]->getType() == "NPC") {
        std::cout << "You should not be moving on to a NPC" << std::endl;
    } else if (theGrid[cdn.x][cdn.y]->getName() == "Treasure") {
        std::cout << "You need to beat the dragon to unlock this item" << std::endl;
    } else if (theGrid[cdn.x][cdn.y]->getName() == "BarrierSuit") {
        std::cout << "You need to beat the dragon to unlock this item" std::endl;
    }
    return false;
}

bool Grid::moveTo(Coordinate& newCdn) { // for PC
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

        std::vector<Coordinate> v;
		countNeighbour(PCLocation, v);
        if (v.size() == 1 && (theGrid[v[0].x][v[0].y]->getName() != "Merchant" || Merchant::hatred != 0)) {
            try {
                PCAttack(v[0]);
            }
            catch (std::runtime_error& errorMsg) {
                std::cout << errorMsg.what() << std::endl;
            }
        } else if (v.size() > 0) {
			std::cout << "More than 1 enermy around, please choose one to attack, or press 's' to skip (i.e.: do not attack)" << std::endl;
			for (size_t i = 0; i < v.size(); i++) {
				std::cout << YELLOW <<  "(" << i << "): " << RESET << "Coordinate: " << v[i] << "  " << theGrid[v[i].x][v[i].y]->getName() << std::endl;
			}
			char opt = 'n';
			int numOpt;
			while (true) {
				std::cin >> numOpt;
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin >> opt;
					if (opt != 's') {
						std::cout << "Invalid option, please select again" << std::cout;
						continue;
					}
					break;
				} else {
					if (numOpt < 0 || numOpt >= v.size()) {
						std::cout << "Invalid option, please select again" << std::cout;
						continue;
					}
					break;
				}
			}
			if (opt == 's') {
				return false; // skip the selection (do not attack any NPC)
			} else {
				PCAttack(v[numOpt]);
				return false
			}
        }
    }
}

void Grid::countNeighbour(Coordinate& cdn, std::vector<Coordinate>& v) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!(i == 0 & j == 0) && theGrid[cdn.x + i][cdn.y + j]->getType() == "NPC") {
				v.emplace_back(new Coordinate{cdn.x + i, cdn.y + j});
			}
		}
	}
}

void Grid::PCAttack(Coordinate& cdn) {
    if (theGrid[cdn.x][cdn.y]->getType() == "NPC") {
        int def = theGrid[cdn.x][cdn.y]->getDefence();
        int dmg = theGrid[PCLocation.x][PCLocation.y]->attack(def);
        theGrid[cdn.x][cdn.y]->attacked(dmg);
    } else {
        throw std::runtime_error("There is no NPC at the position you are attacking.");
    }
}
