#include <iostream>
#include <fstream>
#include <vector>
#include "grid.h"
#include "cell.h"

Grid::Grid(std::string fileName) {
    std::ifstream ifs;
    ifs.open(fileName, std::ios::in);
    std::string s;
    while (std::getline(ifs, s)) {
        std::cout << s << std::endl;





    }
}
























Grid::~Grid() {
    for (auto item : theGrid) {
        for (auto cellptr : item) {
            delete cellptr;
        }
        item.clear();
    }
    theGrid.clear();

    for (auto item : backupGrid) {
        for (auto cellptr : item) {
            delete cellptr;
        }
        item.clear();
    }
    backupGrid.clear();
}





