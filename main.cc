#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <chrono>
#include "grid.h"
#include "termcodes.h"
using namespace std;

Coordinate convertCdn(const Coordinate& oldCdn, string direction) {
     if (direction == "no") {
        return Coordinate{oldCdn.x, oldCdn.y - 1};
     } else if (direction == "so") {
        return Coordinate{oldCdn.x, oldCdn.y + 1};
     } else if (direction == "ea") {
        return Coordinate{oldCdn.x - 1, oldCdn.y};
     } else if (direction == "we") {
        return Coordinate{oldCdn.x + 1, oldCdn.y};
     } else if (direction == "ne") {
        return Coordinate{oldCdn.x - 1, oldCdn.y + 1};
     } else if (direction == "nw") {
        return Coordinate{oldCdn.x - 1, oldCdn.y - 1};
     } else if (direction == "se") {
        return Coordinate{oldCdn.x + 1, oldCdn.y + 1};
     } else if (direction == "sw") {
        return Coordinate{oldCdn.x + 1, oldCdn.y - 1};
     } else {
        return oldCdn;
     }
}


void printIntroMsg() {
	std::cout << "Please select your PC character:" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "(1) Human" << YELLOW << "(h)" << RESET << ": HP:140  Attack:20  Defence:20" << std::endl;
    std::cout << "(2) Dwarf" << YELLOW << "(d)" << RESET << ": HP:100  Attack:20  Defence:30    <gold X2>" << std::endl;
    std::cout << "(3) Elves" << YELLOW << "(e)" << RESET << ": HP:140  Attack:30  Defence:10    <negative potions have positive effects>" << std::endl;
    std::cout << "(4) Orc  " << YELLOW << "(o)" << RESET << ": HP:180  Attack:30  Defence:25    <gold X0.5>" << std::endl;
    std::cout << ">>> Please enter 'h', 'd', 'e', or 'o'" << std::endl;
}


int main(int argc, char* argv[]) {
    std::cout << "WELCOME TO THE GAME OF CHAMBERCRAWLER3000+!" << std::endl;
    printIntroMsg();
    char pc;
	// Player character selection
	while (cin >> pc) {
		if (pc == 'h' || pc == 'd' || pc == 'e' || pc == 'o') {
			break;
		} else {
			std::cout << "Invalid input, please input the correct character" << std::endl;
		}
	}
	Grid* g;
	unsigned defaultSeed = std::chrono::system_clock::now().time_since_epoch().count();
	if (argc == 3) {
		string s = argv[2];
		istringstream iss{s};
		unsigned seed;
		iss >> seed;
		g = new Grid{argv[1], seed, pc};
	
	} else if (argc == 2) {
		g = new Grid{argv[1], defaultSeed, pc};
	} else {
		g = new Grid{"defaultFloor.txt", defaultSeed, pc};
	}
	int currFloor = 1;
	std::vector<int> n;
	for (int i = 0; i < 4; i++) {
		n.emplace_back(i + 1);
	}
	std::shuffle(n.begin(), n.end(), std::default_random_engine{defaultSeed});
	int barrierFloor = n[0];
	n.clear();

		
	// Game starts
	char cmd;
	while (cin >> cmd) {
		// Moving Player Character
		if (cmd == 'n' || cmd == 's' || cmd == 'e' || cmd == 'w') {
			try {
				string direction = "";
				direction += cmd;
				cin >> cmd;
				if (cin.fail()) throw "Incorrect direction format!";
				direction += cmd;
				Coordinate destination = convertCdn(g->getPCLocation(), direction);
				if (g->moveTo(destination)) {
					if (currFloor == 5) {
						cout << "You Win! Your Score is: " << PC::totalCoin << endl;
						return;
					}
					currFloor += 1;
					double c = PC::coin;
					delete g;
					if (currFloor == barrierFloor) {
						g = new Grid{argv[1], ++defaultSeed, pc, true, c};
					} else {
						g = new Grid{argv[1], ++defaultSeed, pc, false, c};
					}
				}
			} catch (string& errorMsg) {
				cout << errorMsg << endl;
				continue;
			}
		} else if (cmd == 'u') {
			try{
				string direction;
				cin >> direction;
				if (cin.fail()) throw "Incorrect direction format!";
				Coordinate destination = convertCdn(g->getPCLocation(), direction);
				g->usePotion(destination);
			} catch (runtime_error& errorMsg) {
				cout << errorMsg.what() << endl;
				continue;
			}

		} else if (cmd == 'a') {
			try{
				string direction;
				cin >> direction;
				if (cin.fail()) throw "Incorrect direction format!";
				Coordinate destination = convertCdn(g->getPCLocation(), direction);
				g->PCAttack(destination);
			} catch (runtime_error& errorMsg) {
				cout << errorMsg.what() << endl;
				continue;
			}
		} else if (cmd == 'q') {
			cout << "DEFEATED! (Player quits the game)" << endl;
			break;
		} else if (cmd == 'r') {
			delete g;
			printIntroMsg();

			// Player character reselection
			while (cin >> pc) {
				if (pc == 'h' || pc == 'd' || pc == 'e' || pc == 'o') {
					break;
				} else {
					std::cout << "Invalid input, please input the correct character" << std::endl;
				}
			}
			unsigned defaultSeed = std::chrono::system_clock::now().time_since_epoch().count();
			if (argc == 3) {
				string s = argv[2];
				istringstream iss{s};
				unsigned seed;
				iss >> seed;
				g = new Grid{argv[1], seed, pc};
	
			} else if (argc == 2) {
				g = new Grid{argv[1], defaultSeed, pc};
			} else {
				g = new Grid{"defaultFloor.txt", defaultSeed, pc};
			}
			int currFloor = 1;
			std::vector<int> n;
			for (int i = 0; i < 4; i++) {
				n.emplace_back(i + 1);
			}
			std::shuffle(n.begin(), n.end(), std::default_random_engine{defaultSeed});
			int barrierFloor = n[0];
			n.clear();
			continue;
        }   
        delete g;
        return 0;
    } catch ( ... ) {
        return 1;
    }
}



