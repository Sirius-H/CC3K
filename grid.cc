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
#include "npc.h"
#include "merchant.h"
#include "dragon.h"
#include "vampire.h"
#include "werewolf.h"
#include "troll.h"
#include "goblin.h"
#include "phoenix.h"

// Debugger
void print( std::vector<Coordinate> const &v ) {
	for ( Coordinate i : v ) std::cout << i << std::endl;
}

// Debugger
void print(std::vector<int> const &v) {
    for (int i : v) std::cout << i << std::endl;
}

// returns a random number between 0 and x-1
int randomInt(int x, unsigned seed) { 
    std::vector<int> num;
    for (int i = 0; i < x; i++) {
        num.emplace_back(i);
    }
    std::shuffle(num.begin(), num.end(), std::default_random_engine(seed));
    int answer = num[0];
    num.clear();
    return answer;
}


Grid::Grid(std::string fileName, unsigned seed, char PCName, bool barrierSuit): seed{seed} {
	gameDiffLevel = 1; // default: normal difficulty level
	#ifdef EASYMODE
	gameDiffLevel = 0;
	#endif
	#ifdef MEDIUMMODE
	gameDiffLevel = 1;
	#endif
	#ifdef HARDMODE
	gameDiffLevel = 2;
	#endif
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
        #ifdef SHOWPC
        std::cout << "Human PC created successfully" << std::endl;
        #endif

    } else if (PCName == 'd') {
        theGrid[x1][y1] = new Dwarf{PCchamber[0]};
        #ifdef SHOWPC
        std::cout << "Dwarf PC created successfully" << std::endl;
        #endif
    } else if (PCName == 'e') {
        theGrid[x1][y1] = new Elf{PCchamber[0]};
        #ifdef SHOWPC
        std::cout << "Elf PC created successfully" << std::endl;
        #endif
    } else if (PCName == 'o') {
        theGrid[x1][y1] = new Orc{PCchamber[0]};
        #ifdef SHOWPC
        std::cout << "Orc PC created successfully" << std::endl;
        #endif
    }
    PCLocation = PCchamber[0];

    // Debugger
    #ifdef SHOWPC
    std::cout << *td;
    std::cout << "PC generated successfully" << std::endl << std::endl;
    #endif



    
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
            // Debugger
            setState(std::pair<Coordinate, char>{stairChamber[i], '/'});
            td->notify(*this);

            break;
        }
    }
    // Debugger
    #ifdef SHOWSTAIR
    std::cout << *td;
    std::cout << ">>> Stair generated" << std::endl<< std::endl;
    #endif
    


    // Step 4: potion generations
    // Debugger
    #ifdef SHOWPOTION
    std::cout << "### Potion effect code: 0-Restore Health; 1-Boost Atk; 2-Boost Def; 3-Poison Health; 4-Wound Atk; 5; Wound Def ###" << std::endl << std::endl;
    #endif
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
            #ifdef SHOWPOTION
            std::cout << "Generated Potion: Coordinate: " << potionChamber[i] << "  Effect: " << randomPotionEffect << std::endl;
            #endif

            td->notify(*this);
            break;
		}
        potionChamber.clear();
    }
    // Debugger
    #ifdef SHOWPOTION
    std::cout << *td;
    std::cout << ">>> Potion generated" << std::endl << std::endl;
    #endif


    // Step 5: Gold
	int goldPileNum = 10; // Normal Mode
	if (gameDiffLevel == 0) { // Easy Mode
		goldPileNum = 15;
	} else if (goldPileNum == 2) { // Hard Mode
		goldPileNum = 5;
	}
    for (int i = 0; i < goldPileNum; i++) {
        std::shuffle(num.begin(), num.end(), std::default_random_engine{++seed});
		// Debugger
        print(num);

        std::vector<Coordinate> goldChamber = chambers[num[0]];
        std::shuffle(goldChamber.begin(), goldChamber.end(), std::default_random_engine{seed});
		for (size_t i = 0; i < goldChamber.size(); i++) {
			if (theGrid[goldChamber[i].x][theGrid[goldChamber[i].y]]->getName() == "Floor") {
			/*
            try {
                canMoveTo(goldChamber[i]);
            } catch (std::runtime_error& msg) {
                continue;
            }
            if (theGrid[goldChamber[i].x][goldChamber[i].y]->getName() != "Treasure") {
			*/
                int x3 = goldChamber[i].x;
                int y3 = goldChamber[i].y;
                delete theGrid[x3][y3];
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
                theGrid[x3][y3] = trs;
                // Debugger
                #ifdef SHOWTREASURE
                std::cout << "Generating treasure:  Coordinate: " << goldChamber[i] << "  Treasure code: " << treasure << std::endl;
                #endif

                setState(std::pair<Coordinate, char>{goldChamber[i], 'G'});
                td->notify(*this);
                
                if (treasure == 9) { // if this is a dragon horde, spawn a dragon next to it
                    std::vector<Coordinate> treasureNeighours;
                    for (int m = -1; m <= 1; m++) {
                        for (int n = -1; n <= 1; n++) {
                            if (theGrid[x3 + m][y3 + n]->getName() == "Floor") {
                                treasureNeighours.emplace_back(Coordinate{x3 + m, y3 + n});
                            }
                        }
                    }
                    Coordinate dragonCdn = treasureNeighours[randomInt(treasureNeighours.size(), ++seed)];
                    delete theGrid[dragonCdn.x][dragonCdn.y];
                    theGrid[dragonCdn.x][dragonCdn.y] = new Dragon{dragonCdn, trs};
                    treasureNeighours.clear();
                    setState(std::pair<Coordinate, char>{dragonCdn, 'D'});
                    td->notify(*this);
                    break;
                }
            }
        }
        goldChamber.clear();
    }
    // Debugger
    #ifdef SHOWTREASURE
    std::cout << *td;
    std::cout << ">>> Treasure generated" << std::endl;
    #endif

    /*
    // Step 6: Barrier Suit
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
    */

    // Step 6: NPC generation
    



    //stairChamber.clear();
    PCchamber.clear();
    num.clear();
}



Coordinate& Grid::getPCLocation() {
    return PCLocation;
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
            if (theGrid[i][j]->getType() == "NPC" && theGrid[i][j]->state() == 0) { // NPC state=0 => has not been moved
                NPC* n = dynamic_cast<NPC*>(theGrid[i][j]);

                if ((PCLocation.x == i || PCLocation.x == i + 1 || PCLocation.x == i - 1) && (PCLocation.y == j || PCLocation.y == j - 1 || PCLocation.y == j + 1)) { // if player is within 1 unit, it automatically attacks the player
                    if (theGrid[i][j]->getName() == "Merchant" && Merchant::hatred == 0) { // if this NPC is a merchant and PC hasn't attacked a merchant on this floor, then it does not attack PC
                        continue;
                    }
                    int def = theGrid[PCLocation.x][PCLocation.y]->getDefence();
                    int dmg = theGrid[i][j]->attack(def);
                    theGrid[PCLocation.x][PCLocation.y]->attacked(dmg);
                    n->setMoved();
                } else { // else move one block
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
                            theGrid[i][j] = new Floor{Coordinate{i, j}};
                            n->setMoved();
                            break;
                        }
                    }
                }
            }
        }
    }
}

/*
bool Grid::canMoveToNPC(Coordinate cdn) {
    std::string name = theGrid[cdn.x][cdn.y]->getName();
    if (name == "Floor") {
        return true;
    }
    // 代码已作废：原因：如果move to一个stair所在的格上，就会delete当前的stair
    if (name == "Stair") {
        return !theGrid[cdn.x][cdn.y]->state(); // 如果目前玩家已经拿到了compass (i.e.: stair revealed = true), 则NPC不可以踩在stair上；如果not revealed，就可以当成普通floor随便踩
    }
    return false;
}
*/

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
    if (theGrid[newCdn.x][newCdn.y]->getName() == "Stair") {
        return true;
    }
    try {
        canMoveTo(newCdn);
    } catch (std::runtime_error& msg) {
        throw;
    }
    if (theGrid[newCdn.x][newCdn.y]->getType() == "Item") {
        int code = theGrid[newCdn.x][newCdn.y]->state();
        PC* p = dynamic_cast<PC*>(theGrid[PCLocation.x][PCLocation.y]);
        p->applyEffect(code);
    }
    delete theGrid[newCdn.x][newCdn.y];
    theGrid[newCdn.x][newCdn.y] = theGrid[PCLocation.x][PCLocation.y];
    theGrid[PCLocation.x][PCLocation.y] = new Floor{PCLocation};
    PCLocation = newCdn;

    // auto attack to surrounding NPC
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
            return false; // skip the selection (do not attack any NPC)
        } else {
            PCAttack(v[numOpt]);
            return false;
        }
        return false;
    } else {
        return false;
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
    } else {
        throw std::runtime_error("There is no NPC at the position you are attacking.");
    }
}



void Grid::usePotion(Coordinate cdn) {
    
}

