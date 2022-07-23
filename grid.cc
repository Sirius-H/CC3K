#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <iomanip>
#include <stdexcept>
#include "coordinate.h"
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
#include "compass.h"
#include "npc.h"
#include "merchant.h"
#include "dragon.h"
#include "vampire.h"
#include "werewolf.h"
#include "troll.h"
#include "goblin.h"
#include "phoenix.h"

Coordinate convertCdn(const Coordinate& oldCdn, std::string direction) {
     if (direction == "no") {
        return Coordinate{oldCdn.x - 1, oldCdn.y};
     } else if (direction == "so") {
        return Coordinate{oldCdn.x + 1, oldCdn.y};
     } else if (direction == "ea") {
        return Coordinate{oldCdn.x, oldCdn.y + 1};
     } else if (direction == "we") {
        return Coordinate{oldCdn.x, oldCdn.y - 1};
     } else if (direction == "ne") {
        return Coordinate{oldCdn.x - 1, oldCdn.y + 1};
     } else if (direction == "nw") {
        return Coordinate{oldCdn.x - 1, oldCdn.y - 1};
     } else if (direction == "se") {
        return Coordinate{oldCdn.x + 1, oldCdn.y + 1};
     } else if (direction == "sw") {
        return Coordinate{oldCdn.x + 1, oldCdn.y - 1};
     } else {
		// Debugger
		throw std::runtime_error("Wrong direction!");
        return oldCdn;
     }
}
// Debugger
void print( std::vector<Coordinate> const &v ) {
	for ( Coordinate i : v ) std::cout << i << std::endl;
}

// Debugger
void print(std::vector<int> const &v) {
    for (int i : v) std::cout << i << std::endl;
}

// Helper
std::string codeTranslator(int code) {
    if (code == 0) {
        return "Restore Health";
    } else if (code == 1) {
        return "Boost Attack + 5";
    } else if (code == 2) {
        return "Boost Defence + 5";
    } else if (code == 3) {
        return "Poison Health";
    } else if (code == 4) {
        return "Wound Attach";
    } else if (code == 5) {
        return "Wound Defence";
    } else if (code == 6) {
        return "Treasure: Normal gold pile";
    } else if (code == 7) {
        return "Treasure: Small horde";
    } else if (code == 8) {
        return "Treasure: Merchant horde";
    } else if (code == 9) {
        return "Treasure: Dragon horde";
    } else if (code == 10) {
        return "Barrier Suit";
    } else if (code == 11) {
        return "Compass";
    } else {
        return "";
    }
} 

// Helper
char SymTranslator(std::string code) {
    if  (code == "Vampire") { return 'V'; }
    else if  (code == "Werewolf") { return 'W'; }
    else if  (code == "Goblin") { return 'N'; }
    else if  (code == "Merchant") { return 'M'; }
    else if  (code == "Dragon") { return 'D'; }
    else if  (code == "Phoenix") { return 'X'; }
    else if  (code == "Troll") { return 'T'; }
    else if  (code == "Treasure") { return 'G'; }
    else if  (code == "BarrierSuit") { return 'B'; }
    else { return ' '; }
}


// Helper
std::string directionCalculator(Coordinate oldCdn, Coordinate newCdn) {
    if (newCdn.x == oldCdn.x - 1 && newCdn.y == oldCdn.y - 1) {
        return "Northwest";
    } else if (newCdn.x == oldCdn.x - 1 && newCdn.y == oldCdn.y) {
        return "North";
    } else if (newCdn.x == oldCdn.x - 1 && newCdn.y == oldCdn.y + 1) {
        return "Northeast";
    } else if (newCdn.x == oldCdn.x && newCdn.y == oldCdn.y - 1) {
        return "West";
    } else if (newCdn.x == oldCdn.x && newCdn.y == oldCdn.y + 1) {
        return "East";
    } else if (newCdn.x == oldCdn.x + 1 && newCdn.y == oldCdn.y - 1) {
        return "Southwest";
    } else if (newCdn.x == oldCdn.x + 1 && newCdn.y == oldCdn.y) {
        return "South";
    } else if (newCdn.x == oldCdn.x + 1 && newCdn.y == oldCdn.y + 1) {
        return "Souteast";
    } else {
        return "";
    }
}


// Returns a random number between 0 and x-1
int randomInt(int x, unsigned seed = std::chrono::system_clock::now().time_since_epoch().count()) { 
    std::vector<int> num;
    for (int i = 0; i < x; i++) {
        num.emplace_back(i);
    }
    std::shuffle(num.begin(), num.end(), std::default_random_engine(seed));
    int answer = num[0];
    num.clear();
    return answer;
}







// Default Constructor (initialize the game with random NPC/Items)
Grid::Grid(std::vector<std::string>& theFloor, unsigned seed, char PCName, bool barrierSuit, std::vector<std::string> *flags): seed{seed}, flags{flags} {
    int totalNPC = 20;
    int mode = 0; // -1 easy; 0 normal; 1 hard
    for (auto s : *flags) {
        if (s == "EASYMODE") {
            mode = -1;
            break;
        }
        if (s == "HARDMODE") {
            mode = 1;
            break;
        }
    }
    // Step 1: create an empty grid of cells, create and connect with TextDisplay
    int lineNum = 0;
    std::vector<std::vector<Cell*>> tempGrid;

    for (size_t i = 0; i < theFloor.size(); i++) {
        std::string s = theFloor[i];
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
    //std::cout << *td;


    // Split chambers
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

    int x1 = PCchamber.at(0).x;
    int y1 = PCchamber.at(0).y;
    delete theGrid.at(x1).at(y1);
    if (PCName == 'h') {
        theGrid[x1][y1] = new Human{PCchamber[0]};
        race = "Human";

        for (auto s : *flags) {
            if (s == "SHOWPC") {
                std::cout << "Human PC created successfully" << std::endl;
                break;
            }
        }

    } else if (PCName == 'd') {
        race = "Dwarf";
        theGrid[x1][y1] = new Dwarf{PCchamber[0]};
        for (auto s : *flags) {
            if (s == "SHOWPC") {
                std::cout << "Dwarf PC created successfully" << std::endl;
                break;
            }
        }
    } else if (PCName == 'e') {
        race = "Elf";
        theGrid[x1][y1] = new Elf{PCchamber[0]};
        for (auto s : *flags) {
            if (s == "SHOWPC") {
                std::cout << "Elf PC created successfully" << std::endl;
                break;
            }
        }
    } else if (PCName == 'o') {
        race = "Orc";
        theGrid[x1][y1] = new Orc{PCchamber[0]};
        for (auto s : *flags) {
            if (s == "SHOWPC") {
                std::cout << "Orc PC created successfully" << std::endl;
                break;
            }
        }
    }
    PCLocation = PCchamber[0];

    // Debugger
    for (auto s : *flags) {
        if (s == "SHOWPC") {
            std::cout << *td;
            std::cout << "PC generated successfully" << std::endl << std::endl;
            break;
        }
    }

    // Step 3: Randomly generate Stair
	std::shuffle(num.begin(), num.end(), std::default_random_engine{++seed});
    std::vector<Coordinate> stairChamber = chambers[num[0]];
    std::shuffle(stairChamber.begin(), stairChamber.end(), std::default_random_engine{++seed});
    for (size_t i = 0; i < stairChamber.size(); i++) {
        int x2 = stairChamber[i].x;
        int y2 = stairChamber[i].y;
        if (theGrid[x2][y2]->getName() == "Floor") {
            delete theGrid.at(x2).at(y2);
            theGrid[x2][y2] = new Stair{stairChamber[i]};
            StairLocation = stairChamber[i];
            // Debugger
            setState(std::pair<Coordinate, char>{stairChamber[i], '\\'});
            td->notify(*this);
            break;
        }
    }
	
    // Debugger
    for (auto s : *flags) {
        if (s == "SHOWSTAIR") {
            std::cout << *td;
            std::cout << ">>> Stair generated" << std::endl<< std::endl;
            break;
        }
    }
    
    // Step 4: potion generations
    // Debugger
    for (auto s : *flags) {
        if (s == "SHOWPOTION") {
            std::cout << "### Potion effect code: 0-Restore Health; 1-Boost Atk; 2-Boost Def; 3-Poison Health; 4-Wound Atk; 5; Wound Def ###" << std::endl << std::endl;
            break;
        }
    }
    
    for (int i = 0; i < 10; i++) {
		std::shuffle(num.begin(), num.end(), std::default_random_engine{++seed});
        std::vector<Coordinate> potionChamber = chambers[num[0]];
        std::shuffle(potionChamber.begin(), potionChamber.end(), std::default_random_engine(++seed));
		for (size_t i = 0; i < potionChamber.size(); i++) {
            try {
                canMoveTo(potionChamber[i]);
            } catch (std::runtime_error& msg) {
                continue;
            }
            int x3 = potionChamber[i].x;
            int y3 = potionChamber[i].y;
            delete theGrid[x3][y3];
            int randomPotionEffect = randomInt(6, ++seed);
            theGrid[x3][y3] = new Potion{potionChamber[i], randomPotionEffect};
            setState(std::pair<Coordinate, char>{potionChamber[i], 'P'});
            // Debugger
            for (auto s : *flags) {
                if (s == "SHOWPOTION") {
                    std::cout << "Generated Potion: Coordinate: " << potionChamber[i] << "  Effect: " << codeTranslator(randomPotionEffect) << std::endl;
                    break;
                }
            }
            td->notify(*this);
            break;
		}
        potionChamber.clear();
    }
    // Debugger

    for (auto s : *flags) {
        if (s == "SHOWPOTION") {
            std::cout << *td;
            std::cout << ">>> Potion generated" << std::endl << std::endl;
            break;
        }
    }


    // Step 5: Gold
	int goldPileNum = 10; // Normal Mode
	if (mode == -1) { // Easy Mode
		goldPileNum = 15;
	} else if (mode == 1) { // Hard Mode
		goldPileNum = 5;
	}
    // Debugger
    for (auto s : *flags) {
        if (s == "SHOWTREASURE") {
            std::cout << "### Treasure code:  6-Normal gold pile;  7-Small horde;  8-Merchant horde;  9-Dragon horde" << std::endl << std::endl;
            break;
        }
    }
    for (int i = 0; i < goldPileNum; i++) {
        std::shuffle(num.begin(), num.end(), std::default_random_engine{++seed});

        std::vector<Coordinate> goldChamber = chambers[num[0]];
        std::shuffle(goldChamber.begin(), goldChamber.end(), std::default_random_engine{seed});
		for (size_t i = 0; i < goldChamber.size(); i++) {
			if (theGrid[goldChamber[i].x][goldChamber[i].y]->getName() == "Floor") {
                int x4 = goldChamber[i].x;
                int y4 = goldChamber[i].y;
                delete theGrid[x4][y4];
                int ri = randomInt(8, ++seed);
                int treasure = 0;
                if (ri <= 4) {
                    treasure = 6;
                } else if (ri == 5) {
                    treasure = 9;
                } else if (ri > 5) {
                    treasure = 7;
                }
                Treasure* trs = new Treasure{goldChamber[i], treasure};
                theGrid[x4][y4] = trs;
                // Debugger
                for (auto s : *flags) {
                    if (s == "SHOWTREASURE") {
                        std::cout << "Generating treasure:  Coordinate: " << goldChamber[i] << "  Treasure code: " << treasure << std::endl;
                        break;
                    }
                }

                setState(std::pair<Coordinate, char>{goldChamber[i], 'G'});
                td->notify(*this);
                
                if (treasure == 9) { // if this is a dragon horde, spawn a dragon next to it
                    std::vector<Coordinate> treasureNeighbours;
                    for (int m = -1; m <= 1; m++) {
                        for (int n = -1; n <= 1; n++) {
                            if (theGrid[x4 + m][y4 + n]->getName() == "Floor") {
                                treasureNeighbours.emplace_back(Coordinate{x4 + m, y4 + n});
                            }
                        }
                    }
                    // Debugger
                    //std::cout << "Treasure neighbour list: " << std::endl;
                    //print(treasureNeighbours);

					if ((int)(treasureNeighbours.size()) == 0) {
						// Debugger
						std::cout << "No place to put the dragon, treasure deleted" << std::endl;
						delete theGrid[x4][y4];
						theGrid[x4][y4] = new Floor{Coordinate{x4, y4}};
					} else {
						Coordinate dragonCdn = treasureNeighbours[randomInt(treasureNeighbours.size(), ++seed)];
	                    delete theGrid[dragonCdn.x][dragonCdn.y];
                        Dragon *d = new Dragon{dragonCdn, trs};
                        if (mode == -1) {
                            d->halfHP();
                        } else if (mode == 1) {
                            d->moreAtk();
                        }
	                    theGrid[dragonCdn.x][dragonCdn.y] = d;
	                    setState(std::pair<Coordinate, char>{dragonCdn, 'D'});
	                    td->notify(*this);
                        totalNPC--;
					}
					treasureNeighbours.clear();
                }
			break;
            }
        }
        goldChamber.clear();
    }
    // Debugger
    for (auto s : *flags) {
        if (s == "SHOWTREASURE") {
            std::cout << *td;
            std::cout << ">>> Treasure generated" << std::endl;
            break;
        }
    }


    // Step 6: Barrier Suit
    if (barrierSuit) {
        std::shuffle(num.begin(), num.end(), std::default_random_engine(++seed));
        std::vector<Coordinate> bsChamber = chambers[num[0]];
        std::shuffle(bsChamber.begin(), bsChamber.end(), std::default_random_engine(++seed));
        for (size_t i = 0; i < bsChamber.size(); i++) {
            int x5 = bsChamber[i].x;
            int y5 = bsChamber[i].y;
            std::vector<Coordinate> bsNeighbours;
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    if (theGrid[x5 + m][y5 + n]->getName() == "Floor") {
                        bsNeighbours.emplace_back(Coordinate{x5 + m, y5 + n});
                    }
                }
            }
            if ((int)(bsNeighbours.size()) == 0) { // if there is no place around the barrier suit for a dragon, then keep searching
                bsNeighbours.clear();
                continue;
            }
            // Creating BarrierSuit in the Grid
            delete theGrid[x5][y5];
            BarrierSuit* bs = new BarrierSuit{bsChamber[i]};
            theGrid[x5][y5] = bs;
            setState(std::pair<Coordinate, char>{bsChamber[i], 'B'});
            td->notify(*this);

            // Create a Dragon around the BarrierSuit
            delete theGrid[bsNeighbours[0].x][bsNeighbours[0].y];
            Dragon *d = new Dragon{bsNeighbours[0], bs};
            if (mode == -1) {
                d->halfHP();
            } else if (mode == 1) {
                d->moreAtk();
            }
            theGrid[bsNeighbours[0].x][bsNeighbours[0].y] = d;
            setState(std::pair<Coordinate, char>{bsNeighbours[0], 'D'});
            td->notify(*this);
            bsNeighbours.clear();
            totalNPC--;
            break;
        }
    }

    // Step 7: NPC generation
    int compassCarrierIndex = randomInt(totalNPC, ++seed);
    bool withCompass = false;

    bool showNPC = false;
    char type;
    for (auto s : *flags) {
        if (s == "SHOWNPC") {
            std::cout << "### NPC code:  W(werewolf);  V(vampire);  N(Goblin);  T(troll);  X(phoenix);  M(merchant)" << std::endl << std::endl;
            showNPC = true;
            break;
        }
    }
    for (int i = 0; i < totalNPC; i++) {
        std::shuffle(num.begin(), num.end(), std::default_random_engine{++seed});
        std::vector<Coordinate> npcChamber = chambers[num[0]];
        std::shuffle(npcChamber.begin(), npcChamber.end(), std::default_random_engine{seed});
        if (i == compassCarrierIndex) {
            withCompass = true;
        } else {
            withCompass = false;
        }
        for (size_t i = 0; i < npcChamber.size(); i++) {
            if (theGrid[npcChamber[i].x][npcChamber[i].y]->getName() == "Floor") {
                int x5 = npcChamber[i].x;
                int y5 = npcChamber[i].y;
                delete theGrid[x5][y5];
                int ri = randomInt(18, ++seed);
                NPC *n;
                if (ri < 4) {
                    n = new Werewolf(npcChamber[i]);
                    setState(std::pair<Coordinate, char>{npcChamber[i], 'W'});
                    if (showNPC) {
                        type = 'W';
                    }
                } else if (ri < 7) {
                    n = new Vampire(npcChamber[i]);
                    setState(std::pair<Coordinate, char>{npcChamber[i], 'V'});
                    if (showNPC) {
                        type = 'V';
                    }
                } else if (ri < 12) {
                    n = new Goblin(npcChamber[i]);
                    setState(std::pair<Coordinate, char>{npcChamber[i], 'N'});
                    if (showNPC) {
                        type = 'N';
                    }
                } else if (ri < 14) {
                    n = new Troll(npcChamber[i]);
                    setState(std::pair<Coordinate, char>{npcChamber[i], 'T'});
                    if (showNPC) {
                        type = 'T';
                    }
                } else if (ri < 16) {
                    n = new Phoenix(npcChamber[i]);
                    setState(std::pair<Coordinate, char>{npcChamber[i], 'X'});
                    if (showNPC) {
                        type = 'X';
                    }
                } else if (ri < 18) {
                    n = new Merchant(npcChamber[i]);
                    setState(std::pair<Coordinate, char>{npcChamber[i], 'M'});
                    if (showNPC) {
                        type = 'M';
                    }
                }
                if (showNPC) {
                    if (!withCompass) {
                        std::cout << "Generated NPC: Coordinate: " << npcChamber[i] << "  Type: " << type << std::endl;
                    } else {
                        std::cout << "Generated NPC: Coordinate: " << npcChamber[i] << "  Type: " << type << GREEN << "  With Compass" << RESET << std::endl;
                    }
                }
                td->notify(*this);
                n->setWithCompass(withCompass);
                if (mode == -1) {
                    n->halfHP();
                } else if (mode == 1) {
                    n->moreAtk();
                }
                theGrid[x5][y5] = n;
                break;
            }
        }
        npcChamber.clear();
    }



    stairChamber.clear();
    PCchamber.clear();
	
    num.clear();

    // Debugger
    /*
    delete theGrid[18][7];
    theGrid[18][7] = new Compass{Coordinate{18,7}};
    setState(std::pair<Coordinate,char>{Coordinate{18,7}, 'C'});
    td->notify(*this);
    */
}







// Another Constructor (read map)
// Constructor (loading saved game)
Grid::Grid(std::vector<std::string>& theFloor, unsigned seed, char PCName, std::vector<std::string>* flags): seed{seed}, flags{flags} {

    std::vector<std::vector<Cell*>> tempGrid;
    // Step 1: create an empty grid of cells, create and connect with TextDisplay
    h = theFloor.size();
    w = theFloor.at(0).size();
    std::vector<std::vector<char>> backupMap;
    for (int i = 0; i < h; i++) {
        std::string s = theFloor[i];
        std::vector<Cell*> tempRow1;
        std::vector<Cell*> tempRow2;
        std::vector<char> backupRow;
        for (int j = 0; j < w; j++) {
            Coordinate currCdn{i, j};
            Cell* ptr1;
            Cell* ptr2;
            if (s[j] == '|') {
                ptr1 = new Wall{currCdn, 1};
                ptr2 = new Wall{currCdn, 1};
                backupRow.emplace_back('|');
            } else if (s[j] == '-') {
                ptr1 = new Wall{currCdn, 2};
                ptr2 = new Wall{currCdn, 2};
                backupRow.emplace_back('-');
            } else if (s[j] == ' ') {
                ptr1 = new Wall{currCdn, 3};
                ptr2 = new Wall{currCdn, 3};
                backupRow.emplace_back(' ');
            } else if (s[j] == '#') {
                ptr1 = new Passage{currCdn, 1};
                ptr2 = new Passage{currCdn, 1};
                backupRow.emplace_back('#');
            } else if (s[j] == '+') {
                ptr1 = new Passage{currCdn, 2};
                ptr2 = new Passage{currCdn, 2};
                backupRow.emplace_back('+');
            } else {
                ptr1 = new Floor{currCdn};
                ptr2 = new Floor{currCdn};
                backupRow.emplace_back('.');
            }
            tempRow1.emplace_back(ptr1);
            tempRow2.emplace_back(ptr2);
        }
        theGrid.emplace_back(tempRow1);
        tempGrid.emplace_back(tempRow2);
        backupMap.emplace_back(backupRow);
    }
    td = new TextDisplay{theGrid};

    // Generate Chambers
    int height = tempGrid.size();
    for (int i = 0; i < height; i++) {
        int width = tempGrid[i].size();
        for (int j = 0; j < width; j++) {
            delete tempGrid[i][j];
        }
        tempGrid[i].clear();
    }
    tempGrid.clear();

    // Step 2: Generate PC/NPC/Items
    bool showPotion = false;
    bool showTreasure = false;
    bool showPC = false;
    bool showNPC = false;
    int mode = 0;
    for (auto s : *flags) {
        if (s == "SHOWPOTION") {
            showPotion = true;
            showTreasure = true;
            showPC = true;
            showNPC = true;
        }
        if (s == "SHOWTREASURE") {
            showTreasure = true;
        }
        if (s == "SHOWPC") {
            showPC = true;
        }
        if (s == "SHOWNPC") {
            showNPC = true;
        }
        if (s == "MOREMONEY") {
            PC::coin = 9999;
        }
        if (s == "EASYMODE") {
            mode = -1;
        }
        if (s == "HARDMODE") {
            mode = 1;
        }
    }
    bool foundCompass = false;
    std::vector<NPC*> v;
    for (int i = 0; i < h; i++) {
        std::string s = theFloor[i];
        for (int j = 0; j < w; j++) {
            Coordinate currCdn{i, j};
            if (s[j] == '0') {
                delete theGrid[i][j];
                theGrid[i][j] = new Potion(currCdn, 0);
                setState(std::pair<Coordinate, char>{currCdn, 'P'});
                if (showPotion) {
                    std::cout << "Generated Potion: Coordinate: " << currCdn << "  Effect: " << codeTranslator(0) << std::endl;
                }
            } else if (s[j] == '1') {
                delete theGrid[i][j];
                theGrid[i][j] = new Potion(currCdn, 1);
                setState(std::pair<Coordinate, char>{currCdn, 'P'});
                if (showPotion) {
                    std::cout << "Generated Potion: Coordinate: " << currCdn << "  Effect: " << codeTranslator(1) << std::endl;
                }
            } else if (s[j] == '2') {
                delete theGrid[i][j];
                theGrid[i][j] = new Potion(currCdn, 2);
                setState(std::pair<Coordinate, char>{currCdn, 'P'});
                if (showPotion) {
                    std::cout << "Generated Potion: Coordinate: " << currCdn << "  Effect: " << codeTranslator(2) << std::endl;
                }
            } else if (s[j] == '3') {
                delete theGrid[i][j];
                theGrid[i][j] = new Potion(currCdn, 3);
                setState(std::pair<Coordinate, char>{currCdn, 'P'});
                if (showPotion) {
                    std::cout << "Generated Potion: Coordinate: " << currCdn << "  Effect: " << codeTranslator(3) << std::endl;
                }
            } else if (s[j] == '4') {
                delete theGrid[i][j];
                theGrid[i][j] = new Potion(currCdn, 4);
                setState(std::pair<Coordinate, char>{currCdn, 'P'});
                if (showPotion) {
                    std::cout << "Generated Potion: Coordinate: " << currCdn << "  Effect: " << codeTranslator(4) << std::endl;
                }
            } else if (s[j] == '5') {
                delete theGrid[i][j];
                theGrid[i][j] = new Potion(currCdn, 5);
                setState(std::pair<Coordinate, char>{currCdn, 'P'});
                if (showPotion) {
                    std::cout << "Generated Potion: Coordinate: " << currCdn << "  Effect: " << codeTranslator(5) << std::endl;
                }
            } else if (s[j] == '6') {
                delete theGrid[i][j];
                theGrid[i][j] = new Treasure(currCdn, 6);
                setState(std::pair<Coordinate, char>{currCdn, 'G'});
                if (showPotion) {
                    std::cout << "Generated Potion: Coordinate: " << currCdn << "  Effect: " << codeTranslator(6) << std::endl;
                }
            } else if (s[j] == '7') {
                delete theGrid[i][j];
                theGrid[i][j] = new Treasure(currCdn, 7);
                setState(std::pair<Coordinate, char>{currCdn, 'G'});
                if (showTreasure) {
                    std::cout << "Generating treasure:  Coordinate: " << currCdn << "  Treasure code: " << 7 << std::endl;
                }
            } else if (s[j] == '8') {
                delete theGrid[i][j];
                
                theGrid[i][j] = new Treasure(currCdn, 8);
                setState(std::pair<Coordinate, char>{currCdn, 'G'});
                if (showTreasure) {
                    std::cout << "Generating treasure:  Coordinate: " << currCdn << "  Treasure code: " << 8 << std::endl;
                }
            } else if (s[j] == '9') {
                delete theGrid[i][j];
                Treasure *t = new Treasure(currCdn, 9);
                theGrid[i][j] = t;
                setState(std::pair<Coordinate, char>{currCdn, 'G'});
                td->notify(*this);
                if (showTreasure) {
                    std::cout << "Generating treasure:  Coordinate: " << currCdn << "  Treasure code: " << 9 << std::endl;
                }
                bool foundDragon = false;
                for (int w = -1; w <= 1; w++) {
                    for (int h = -1; h <= 1; h++) {
                        if (theFloor[i+w][j+h] == 'D') {
                            delete theGrid[i+w][j+h];
                            Dragon *d = new Dragon{Coordinate{i+w, j+h}, t};
                            setState(std::pair<Coordinate, char>{Coordinate{i+w, j+h}, 'D'});
                            if (mode == -1) {
                                d->halfHP();
                            } else if (mode == 1) {
                                d->moreAtk();
                            }
                            theGrid[i+w][j+h] = d;
                            foundDragon = true;
                            break;
                        }
                    }
                    if (foundDragon) {
                        break;
                    }
                }
                if (!foundDragon) {
                    Dragon *d = new Dragon{Coordinate{0, 0}, t};
                    d->notifyObserver();
                    delete d;
                }
            } else if (s[j] == 'B') {
                delete theGrid[i][j];
                BarrierSuit *b = new BarrierSuit(currCdn);
                theGrid[i][j] = b;
                setState(std::pair<Coordinate, char>{currCdn, 'B'});
                td->notify(*this);
                bool foundDragon = false;
                for (int w = -1; w <= 1; w++) {
                    for (int h = -1; h <= 1; h++) {
                        if (theFloor[i+w][j+h] == 'D') {
                            delete theGrid[i+w][j+h];
                            Dragon *d = new Dragon{Coordinate{i+w, j+h}, b};
                            setState(std::pair<Coordinate, char>{Coordinate{i+w, j+h}, 'D'});
                            if (mode == -1) {
                                d->halfHP();
                            } else if (mode == 1) {
                                d->moreAtk();
                            }
                            theGrid[i+w][j+h] = d;
                        }
                    }
                    if (foundDragon) {
                        break;
                    }
                }
                if (!foundDragon) {
                    Dragon *d = new Dragon{Coordinate{0, 0}, b};
                    d->notifyObserver();
                    delete d;
                }
                theGrid[i][j] = b;
            } else if (s[j] == '@') {
                delete theGrid[i][j];
                if (PCName == 'h') {
                    theGrid[i][j] = new Human{currCdn};
                    race = "Human";
                    if (showPC) {
                        std::cout << "Human PC created successfully" << std::endl;
                    }
                } else if (PCName == 'o') {
                    theGrid[i][j] = new Orc{currCdn};
                    race = "Orc";
                    if (showPC) {
                        std::cout << "Orc PC created successfully" << std::endl;
                    }
                } else if (PCName == 'd') {
                    theGrid[i][j] = new Dwarf{currCdn};
                    race = "Dwarf";
                    if (showPC) {
                        std::cout << "Dwarf PC created successfully" << std::endl;
                    }
                } else if (PCName == 'e') {
                    theGrid[i][j] = new Elf{currCdn};
                    race = "Elf";
                    if (showPC) {
                        std::cout << "Elf PC created successfully" << std::endl;
                    }
                }
                PCLocation = currCdn;
                setState(std::pair<Coordinate, char>{currCdn, '@'});
            } else if (s[j] == '\\') {
                delete theGrid[i][j];
                theGrid[i][j] = new Stair{currCdn};
                StairLocation = currCdn;
                setState(std::pair<Coordinate, char>{currCdn, '\\'});
            } else if (s[j] == 'C') {
                delete theGrid[i][j];
                theGrid[i][j] = new Compass{currCdn};
                setState(std::pair<Coordinate, char>{currCdn, 'C'});
                foundCompass = true;
            } else if (s[j] == 'V') {
                delete theGrid[i][j];
                NPC *n = new Vampire(currCdn);
                if (mode == -1) {
                    n->halfHP();
                }
                theGrid[i][j] = n;
                v.emplace_back(n);
                setState(std::pair<Coordinate, char>{currCdn, 'V'});
                if (showNPC) {
                    std::cout << "Generated NPC: Coordinate: " << currCdn << "  Type: " << 'V' << std::endl;
                }
            } else if (s[j] == 'N') {
                delete theGrid[i][j];
                NPC *n = new Goblin(currCdn);
                if (mode == -1) {
                    n->halfHP();
                } else if (mode == 1) {
                    n->moreAtk();
                }
                theGrid[i][j] = n;
                v.emplace_back(n);
                setState(std::pair<Coordinate, char>{currCdn, 'N'});
                if (showNPC) {
                    std::cout << "Generated NPC: Coordinate: " << currCdn << "  Type: " << 'N' << std::endl;
                }
            } else if (s[j] == 'X') {
                delete theGrid[i][j];
                NPC *n = new Phoenix(currCdn);
                if (mode == -1) {
                    n->halfHP();
                } else if (mode == 1) {
                    n->moreAtk();
                }
                theGrid[i][j] = n;
                v.emplace_back(n);
                setState(std::pair<Coordinate, char>{currCdn, 'X'});
                if (showNPC) {
                    std::cout << "Generated NPC: Coordinate: " << currCdn << "  Type: " << 'X' << std::endl;
                }
            } else if (s[j] == 'W') {
                delete theGrid[i][j];
                NPC *n = new Werewolf(currCdn);
                if (mode == -1) {
                    n->halfHP();
                } else if (mode == 1) {
                    n->moreAtk();
                }
                theGrid[i][j] = n;
                v.emplace_back(n);
                setState(std::pair<Coordinate, char>{currCdn, 'W'});
                if (showNPC) {
                    std::cout << "Generated NPC: Coordinate: " << currCdn << "  Type: " << 'W' << std::endl;
                }
            } else if (s[j] == 'T') {
                delete theGrid[i][j];
                NPC *n = new Troll(currCdn);
                if (mode == -1) {
                    n->halfHP();
                } else if (mode == 1) {
                    n->moreAtk();
                }
                theGrid[i][j] = n;
                v.emplace_back(n);
                setState(std::pair<Coordinate, char>{currCdn, 'T'});
                if (showNPC) {
                    std::cout << "Generated NPC: Coordinate: " << currCdn << "  Type: " << 'T' << std::endl;
                }
            } else if (s[j] == 'M') {
                delete theGrid[i][j];
                NPC *n = new Merchant(currCdn);
                if (mode == -1) {
                    n->halfHP();
                } else if (mode == 1) {
                    n->moreAtk();
                }
                theGrid[i][j] = n;
                v.emplace_back(n);
                setState(std::pair<Coordinate, char>{currCdn, 'M'});
                if (showNPC) {
                    std::cout << "Generated NPC: Coordinate: " << currCdn << "  Type: " << 'M' << std::endl;
                }
            } else {
                setState(std::pair<Coordinate, char>{currCdn, backupMap[i][j]});
            }
            td->notify(*this);
            if (showPC) {
                std::cout << *td;
                std::cout << "PC generated successfully" << std::endl << std::endl;
            }
        }
    }
    if (!foundCompass) {
        int size = v.size();
        std::vector<int> num;
        for (int i = 0; i < size; i++) {
            num.emplace_back(i);
        }
        std::shuffle(num.begin(), num.end(), std::default_random_engine{seed});
        v[num.at(0)]->setWithCompass(true);
        num.clear();
    }
    v.clear();
}

// Destructor
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
    for (auto s : *flags) {
        if (s == "MOREMONEY") {
            PC::coin = 999.00;
        }
    }
    std::cout << "h: " << h << std::endl;
    std::cout << "w:" << w << std::endl;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (theGrid[i][j]->getType() == "NPC" && theGrid[i][j]->state() == NPC::currInitState) { // NPC state=0 => has not been moved

                NPC* n = dynamic_cast<NPC*>(theGrid[i][j]);

                if (theGrid[i][j]->getHP() <= 0) { // If this NPC dies, remove it from the grid
                    actionLog.emplace_back(theGrid[i][j]->getName() + " is eliminated.");
                    PC* p = dynamic_cast<PC*>(theGrid[PCLocation.x][PCLocation.y]);
                    p->applyElimNPCAward();

                    if (theGrid[i][j]->getName() == "Dragon") {
                        Dragon* drg = dynamic_cast<Dragon*>(theGrid[i][j]);
                        drg->notifyObserver();
                        // Debugger
                        actionLog.emplace_back("The treasure it guards has been unlocked");
                    }

                    if (n->getWithCompass()) {
                        delete theGrid[i][j];
                        theGrid[i][j] = new Compass{Coordinate{i,j}};
                        setState(std::pair<Coordinate, char>{Coordinate{i, j}, 'C'});
                    } else {
                        delete theGrid[i][j];
                        theGrid[i][j] = new Floor{Coordinate{i, j}};
                        setState(std::pair<Coordinate, char>{Coordinate{i, j}, '.'});
                    }
                    td->notify(*this);
                    continue;
                }

                else if ((PCLocation.x == i || PCLocation.x == i + 1 || PCLocation.x == i - 1) && (PCLocation.y == j || PCLocation.y == j - 1 || PCLocation.y == j + 1)) { // If player is within 1 unit, it automatically attacks the player
                    if (theGrid[i][j]->getName() == "Merchant" && Merchant::hatred == 0) { // if this NPC is a merchant and PC hasn't attacked a merchant on this floor, then it does not attack PC
                        continue;
                    }
                    int def = theGrid[PCLocation.x][PCLocation.y]->getDefence();
                    int dmg = n->attack(def);
                    theGrid[PCLocation.x][PCLocation.y]->attacked(dmg);
                    actionLog.emplace_back(theGrid[i][j]->getName() + " deals " + std::to_string(dmg) + " HP damage to PC.");
                    n->setMoved();
                } 
                                
                
                else { // else move one block
                    if (theGrid[i][j]->getName() == "Dragon") {
                        n->setMoved();
                        continue;
                    } // Dragons do not move

                    std::vector<Coordinate> v;
                    for (int m = -1; m <= 1; m++) {
                        for (int n = -1; n <= 1; n++) {
                            if (m == 0 && n == 0) {
                                continue;
                            }
                            v.emplace_back(Coordinate{i + m, j + n});
                        }
                    }
                    std::shuffle(v.begin(), v.end(), std::default_random_engine(++seed));
                    for (int k = 0; k < 8; k++) {
                        if (theGrid[v[k].x][v[k].y]->getName() == "Floor") {
                            delete theGrid[v[k].x][v[k].y];
                            theGrid[v[k].x][v[k].y] = theGrid[i][j];
                            theGrid[v[k].x][v[k].y]->setCdn(v[k]);
                            setState(std::pair<Coordinate, char>{v[k], SymTranslator(theGrid[v[k].x][v[k].y]->getName())});
                            td->notify(*this);
                            theGrid[i][j] = new Floor{Coordinate{i, j}};
                            setState(std::pair<Coordinate, char>{Coordinate{i, j}, '.'});
                            td->notify(*this);
                            n->setMoved();
                            break;
                        }
                    }
                }
            }
        }
    }
    NPC::currInitState = 1 - NPC::currInitState;
}



bool Grid::canMoveTo(Coordinate cdn) { // for PC
    if (theGrid[cdn.x][cdn.y]->canStep() == true) {
        return true;
    } else if (theGrid[cdn.x][cdn.y]->getName() == "Wall") {
        throw std::runtime_error("You should not be moving on to a wall");
    } else if (theGrid[cdn.x][cdn.y]->getName() == "Potion") {
        throw std::runtime_error("You should not be moving on to a potion");
    } else if (theGrid[cdn.x][cdn.y]->getType() == "NPC") {
        throw std::runtime_error("You should not be moving on to a NPC");
    } else if (theGrid[cdn.x][cdn.y]->getName() == "Treasure") {
        throw std::runtime_error("You need to beat the dragon to unlock this item");
    } else if (theGrid[cdn.x][cdn.y]->getName() == "BarrierSuit") {
        throw std::runtime_error("You need to beat the dragon to unlock this item");
    }
    return false;
}

bool Grid::moveTo(Coordinate newCdn) { // for PC
    // Debugger
    //std::cout << "PC Location: " << PCLocation << std::endl;
    //std::cout << "Target Cdn:" << newCdn << std::endl;
    std::string msg = "";

    if (theGrid[newCdn.x][newCdn.y]->getName() == "Stair") {
        return true;
    }
    try {
        canMoveTo(newCdn);
    } catch (std::runtime_error& msg) {
        throw;
    }
    msg = msg + "PC moved to " + directionCalculator(PCLocation, newCdn);

    if (theGrid[newCdn.x][newCdn.y]->getType() == "Item") {
        int code = theGrid[newCdn.x][newCdn.y]->state();
        // Debugger
        std::cout << MAGENTA << "Item: " << codeTranslator(code) << RESET << std::endl;
        msg = msg + " and collected " + codeTranslator(code);

        PC* p = dynamic_cast<PC*>(theGrid[PCLocation.x][PCLocation.y]);
        p->applyEffect(code);
        if (code == 11) { // If PC moves onto a Compass, then the Stair is revealed
            setState(std::pair<Coordinate, char>{StairLocation, '\\'});
            td->notify(*this);
            // Debugger
            //std::cout << GREEN << "You had the compass, stair revealed >>>" << RESET << std::endl;
            actionLog.emplace_back("PC obtains the Compass. Stair is now revealed.");
        }
    }
    std::string name = theGrid[newCdn.x][newCdn.y]->getName();
    int state = theGrid[newCdn.x][newCdn.y]->state();
    delete theGrid[newCdn.x][newCdn.y];
    theGrid[newCdn.x][newCdn.y] = theGrid[PCLocation.x][PCLocation.y];
    theGrid[newCdn.x][newCdn.y]->setCdn(newCdn);
    setState(std::pair<Coordinate,char>{newCdn, '@'});
    td->notify(*this);
    if (PC::onTile == 1) {
        theGrid[PCLocation.x][PCLocation.y] = new Passage{PCLocation, PC::onTile};
        setState(std::pair<Coordinate,char>{PCLocation, '#'});
    } else if (PC::onTile == 2) {
        theGrid[PCLocation.x][PCLocation.y] = new Passage{PCLocation, PC::onTile};
        setState(std::pair<Coordinate,char>{PCLocation, '+'});
    } else {
        theGrid[PCLocation.x][PCLocation.y] = new Floor{PCLocation};
        setState(std::pair<Coordinate,char>{PCLocation, '.'});
    }
    PC::onTile = state;
    td->notify(*this);
    PCLocation = newCdn;

    // auto attack to surrounding NPC
    std::vector<Coordinate> v;
    countNeighbour(PCLocation, v);
    if (v.size() == 0) {
        actionLog.emplace_back(msg + ".");
    } else if (v.size() == 1 && (theGrid[v[0].x][v[0].y]->getName() != "Merchant" || Merchant::hatred != 0)) {
        msg = msg + " and meets 1 NPC: " + theGrid[v[0].x][v[0].y]->getName();
        actionLog.emplace_back(msg + ".");
        try {
            PCAttack(v[0]);
        }
        catch (std::runtime_error& errorMsg) {
            std::cout << errorMsg.what() << std::endl;
        }
    } else if (v.size() > 1) {
        msg = msg + " and meets more than 1 NPC.";
        actionLog.emplace_back(msg + ".");
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
                    std::cout << "Invalid option, please select again" << std::endl;
                    continue;
                }
                break;
            } else {
                if (numOpt < 0 || numOpt >= (int)v.size()) {
                    std::cout << "Invalid option, please select again" << std::endl;
                    continue;
                }
                break;
            }
        }
        if (opt == 's') {
            v.clear();
            return false; // skip the selection (do not attack any NPC)
        } else {
            v.clear();
            PCAttack(v[numOpt]);
            return false;
        }
        v.clear();
        return false;
    } 
    v.clear();

    int potionCount = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (theGrid[PCLocation.x + i][PCLocation.y + j]->getName() == "Potion") {
                potionCount++;
            }
        }
    }
    if (potionCount > 0) {
        actionLog.emplace_back("PC meets " + std::to_string(potionCount) + " unknown Potion.");
    }
    return false;
}


void Grid::countNeighbour(Coordinate& cdn, std::vector<Coordinate>& v) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (!(i == 0 && j == 0) && theGrid[cdn.x + i][cdn.y + j]->getType() == "NPC") {
				v.emplace_back(Coordinate{cdn.x + i, cdn.y + j});
			}
		}
	}
}


void Grid::PCAttack(Coordinate cdn) {
    if (theGrid[cdn.x][cdn.y]->getType() == "NPC") {
        int def = theGrid[cdn.x][cdn.y]->getDefence();
        int dmg = theGrid[PCLocation.x][PCLocation.y]->attack(def);
        theGrid[cdn.x][cdn.y]->attacked(dmg);
        /*
        if (theGrid[cdn.x][cdn.y]->getHP() <= 0) {
            delete theGrid[cdn.x][cdn.y];
            theGrid[cdn.x][cdn.y] = new Floor{cdn};
        }
        */
        actionLog.emplace_back("PC deals " + std::to_string(dmg) + " HP damage to " + theGrid[cdn.x][cdn.y]->getName() + ".");
    } else {
        throw std::runtime_error("There is no NPC at the position you are attacking.");
    }
}



void Grid::usePotion(Coordinate cdn) {
    if (theGrid[cdn.x][cdn.y]->getName() == "Potion") {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) {
                    continue;
                } else if (cdn.x + i == PCLocation.x && cdn.y + j == PCLocation.y) {
                    int code = theGrid[cdn.x][cdn.y]->state();
                    PC* p = dynamic_cast<PC*>(theGrid[PCLocation.x][PCLocation.y]);
                    delete theGrid[cdn.x][cdn.y];
                    theGrid[cdn.x][cdn.y] = new Floor{cdn};
                    setState(std::pair<Coordinate,char>{cdn, '.'});
                    td->notify(*this);
                    // Debugger
                    //std::cout << RED << "Used potion at " << cdn  << RESET << std::endl;
                    actionLog.emplace_back("PC uses Potion: " + codeTranslator(code) + ".");

                    try {
                        p->applyEffect(code);
                        // Debugger
                        std::cout << RED << "Effect: " << codeTranslator(code) << RESET << std::endl;
                    }
                    catch (std::runtime_error& errorMsg) {
                        // Debugger
                        std::cout << RED << "Effect: " << errorMsg.what() << RESET << std::endl;
                        actionLog.emplace_back(errorMsg.what());
                    }
                    break;
                }
            }
        }
    } else {
        throw std::runtime_error("There is no Potion at the position you are trying to use potion.");
    }
}

void Grid::printState(int floorNum) const {
    std::cout << *td;
    PC* p = dynamic_cast<PC*>(theGrid[PCLocation.x][PCLocation.y]);
    std::cout << CYAN << "Race: " << race << RESET << "                                                            Floor " << floorNum << std::endl;
    std::cout << YELLOW << "Coin: " << std::setprecision(3) << PC::coin << std::endl;
    std::cout << "Total Score: " << std::setprecision(3) << PC::totalCoin << RESET << std::endl; 
    std::cout << GREEN << "HP: " << p->getHP() << "   " << "Attack: " << p->getAtk() << "   " << "Defence: " << p->getDef() << RESET << std::endl;
    std::cout << CYAN << "Barrier Suit Status:    ";
    if (p->getWithBarrierSuit()) {
        std::cout << GREEN << "<ACQUIRED>" << RESET;
    } else {
        std::cout << RED << "<NOT ACQUIRED>" << RESET;
    }
    std::cout << std::endl;
    std::cout << CYAN << "Compass Status:         ";
    if (p->getWithCompass()) {
        std::cout << GREEN << "<ACQUIRED>" << RESET;
    } else {
        std::cout << RED << "<NOT ACQUIRED>" << RESET;
    }
    std::cout << std::endl;
    std::cout << RED << "NPCs around PC: " << RESET << std::endl;
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (theGrid[PCLocation.x + i][PCLocation.y + j]->getType() == "NPC") {
                NPC* n = dynamic_cast<NPC*>(theGrid[PCLocation.x + i][PCLocation.y + j]);
                std::cout << "  " << RED << n->getName() << " (" << PCLocation.x + i << "," <<  PCLocation.y + j << "): " << "HP: " << n->getHP() << "   "  << "Atk: " << n->getAtk() << "   "  << "Def: " << n->getDef() << RESET << std::endl;
            }
        }
    }
}

void Grid::buyPotion(std::string dir) {
    int x = getPCLocation().x;
    int y = getPCLocation().y;
    std::vector<Coordinate> v;
    char c;
    countNeighbour(PCLocation, v);
    if (Merchant::hatred != 0) return;
    for (size_t i = 0; i < v.size(); i++) { // Freeze all the merchants 1 unit around PC
        if (theGrid[v[i].x][v[i].y]->getName() == "Merchant") {
            dynamic_cast<NPC*>(theGrid[v[i].x][v[i].y])->setMoved();
        }
    }
    Coordinate l = convertCdn(getPCLocation(), dir);
    if (theGrid[l.x][l.y]->getName() == "Merchant") {
		std::cout << "Each potion costs 5 coins" << std::endl;
        for (int i = 0; i < 3; i++) {
            int effect = dynamic_cast<Merchant*>(theGrid[l.x][l.y])->potions[i];
            std::cout << "(" << std::to_string(i + 1) << "): " << codeTranslator(effect) << std::endl;
        }
        std::cout << "Please enter the corresponding number to buy potion, or enter 's' to skip >>>" << std::endl;
        // reading in option
        while (std::cin >> c) {
            if (c == 's') break;
            if (c == '1' || c == '2' || c == '3') {
                if (PC::coin >= 5) { // If PC has enough money to buy this potion
                    PC::coin -= 5;
                    int potionBought = dynamic_cast<Merchant*>(theGrid[l.x][l.y])->potions[c - '1'];
                    dynamic_cast<PC*>(theGrid[x][y])->applyEffect(potionBought);
                    std::cout << "Potion bought successfully." << std::endl;
                } else {
                    std::cout << "Not enough gold! Transaction cancelled." << std::endl;
                }
            } else {
                std::cout << "Incorrect number! Please try again!" << std::endl;
                continue;
            }
			std::cout << "Enter any number to continue, or press s to escape >>>" << std::endl;
        }        
    } else {
        throw std::runtime_error("There is no Merchant at PC's " + directionCalculator(PCLocation, l) + " direction.");
    }
}



int Grid::getHP() {
    return theGrid[PCLocation.x][PCLocation.y]->getHP();
}

Coordinate& Grid::getPCLocation() {
    return PCLocation;
}
