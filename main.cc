#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <algorithm>
#include <random>
#include "grid.h"
#include "termcodes.h"
#include "pc.h"
using namespace std;

Coordinate convertCdn(const Coordinate& oldCdn, string direction) {
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
		throw runtime_error("Wrong direction!");
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
	// Print out welcome message
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

	// Set up seed
	unsigned seed;
	if (argc == 3) {
		string s = argv[2];
		istringstream iss{s};
		iss >> seed;
	} else { // if not specified, use random seed
		seed = std::chrono::system_clock::now().time_since_epoch().count();
	}

	int currFloor = 1; // Current floor number
	std::vector<int> n;
	for (int i = 1; i <= 5; i++) {
		n.emplace_back(i + 1);
	}
	std::shuffle(n.begin(), n.end(), std::default_random_engine{seed});
	int barrierFloor = n[0];
	n.clear();

	// Create new Grid object (game)
	Grid* g;
	if (argc == 3 || argc == 2) {
		if (currFloor == barrierFloor) {
			g = new Grid{argv[1], seed, pc, true};
		} else {
			g = new Grid{argv[1], seed, pc, false};
		}
	} else {
		if (currFloor == barrierFloor) {
			g = new Grid{"defaultFloor.txt", seed, pc, true};
		} else {
			g = new Grid{"defaultFloor.txt", seed, pc, false};
		}
	}
	g->printState(currFloor);
		
	// Game starts
	char cmd;
	while (cin >> cmd) {
		// Moving Player Character
		if (cmd == 'n' || cmd == 's' || cmd == 'e' || cmd == 'w') {
			try {
				string direction = "";
				direction += cmd;
				cin >> cmd;
				if (cin.fail()) throw runtime_error("Incorrect direction format!");
				direction += cmd;
				Coordinate destination = convertCdn(g->getPCLocation(), direction);
				if (g->moveTo(destination)) {
					// Debugger
					std::cout << "Triggered going down stairs" << std::endl;

					if (currFloor == 5) {
						cout << "You Win! Your Score is: " << PC::totalCoin << endl;
						break;
					}
					currFloor += 1;
					delete g;
					if (currFloor == barrierFloor) {
						g = new Grid{argv[1], ++seed, pc, true};
					} else {
						g = new Grid{argv[1], ++seed, pc, false};
					}
				}
				g->updateGrid();
				g->printState(currFloor);
                if (g->getHP() == 0) {
                    cout << "DEFEATED!" << std::endl;
                    break;
                }
				continue;
			} catch (runtime_error& errorMsg) {
				cout << errorMsg.what() << endl;
				continue;
			}
		} else if (cmd == 'u') {
			try{
				string direction;
				cin >> direction;
				if (cin.fail()) throw runtime_error("Incorrect direction format!");
				Coordinate destination = convertCdn(g->getPCLocation(), direction);
				// Debugger
				std::cout << "Current PC Location: " << g->getPCLocation() << std::endl;
				std::cout << "Accessing cdn: " << destination << std::endl;
				std::cout << direction << std::endl;

				g->usePotion(destination);
				g->updateGrid();
				g->printState(currFloor);
                if (g->getHP() == 0) {
                    cout << "DEFEATED!" << std::endl;
                    break;
                }
				continue;
			} catch (runtime_error& errorMsg) {
				cout << errorMsg.what() << endl;
				continue;
			}

		} else if (cmd == 'a') {
			try{
				string dir;
				cin >> dir;
				if (cin.fail() || (dir != "no" && dir != "so" && dir != "we" && dir != "ea" && dir != "nw" && dir != "ne" && dir != "sw" && dir != "se")) {
					throw runtime_error("Incorrect direction format!");
				}
				Coordinate destination = convertCdn(g->getPCLocation(), dir);
				g->PCAttack(destination);
				g->updateGrid();
				g->printState(currFloor);
                if (g->getHP() == 0) {
                    cout << "DEFEATED!" << std::endl;
                    break;
                }
				continue;
			} catch (runtime_error& errorMsg) {
				cout << errorMsg.what() << endl;
				continue;
			}

		} else if (cmd == 'q') {
			cout << "DEFEATED! (Player quits the game)" << endl;
			break;


		} else if (cmd == 'r') {
			currFloor = 1;
			int currFloor = 1; // Current floor number
			std::vector<int> n;
			for (int i = 1; i <= 5; i++) {
				n.emplace_back(i + 1);
			}
			std::shuffle(n.begin(), n.end(), std::default_random_engine{seed});
			int barrierFloor = n[0];
			n.clear();
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
			// Reassign/regenerate seed
			if (argc == 3) {
				string s = argv[2];
				istringstream iss{s};
				iss >> seed;
			} else { // if not specified, use random seed
				seed = std::chrono::system_clock::now().time_since_epoch().count();
			}
			if (argc == 3 || argc == 2) {
				if (currFloor == barrierFloor) {
					g = new Grid{argv[1], seed, pc, true};
				} else {
					g = new Grid{argv[1], seed, pc, false};
				}
			} else {
				if (currFloor == barrierFloor) {
					g = new Grid{"defaultFloor.txt", seed, pc, true};
				} else {
					g = new Grid{"defaultFloor.txt", seed, pc, false};
				}
			}
			g->printState(currFloor);
			continue;
        } else {
			std::cout << "Invalid command, please try again!" << std::endl;
			continue;
		}
        delete g;
        return 0;
    }
}



