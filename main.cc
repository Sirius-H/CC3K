#include <iostream>
#include <fstream>
#include <sstream>
#include "grid.h"
using namespace std;


int main(int argc, char* argv[]) {
    Grid* g;
    if (argc == 3) {
        string s = argv[2];
        istringstream iss{s};
        unsigned seed;
        iss >> seed;
        g = new Grid{argv[1], seed};

    } else if (argc == 2) {
        g = new Grid{argv[1]};
    } else {
        g = new Grid{"defaultFloor.txt"};
    }
    delete g;




}



