#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "grid.h"
#include "cell.h"
#include "mapelement.h"
#include "wall.h"
#include "floor.h"
#include "passage.h"

Grid::Grid(std::string fileName) {
    std::ifstream ifs;
    ifs.open(fileName, std::ios::in);
    std::string s;
    std::map<Coordinate, int> allFloors;
    int lineNum = 0;
    while (std::getline(ifs, s)) { // 目前只实现了floor.txt中仅包含单张地图
        std::cout << s << std::endl;
        std::vector<Cell*> tempRow;
        int len = s.length();
        for (int i = 0; i < len; i++) {
            Coordinate currCdn{lineNum, i};
            Cell* ptr;
            if (s[i] == '|' || s[i] == '-' || s[i] == ' ') {
                ptr = new Wall{currCdn};
            } else if (s[i] == '.') {
                ptr = new Floor{currCdn};
                Coordinate currCdn{lineNum, i};
                allFloors.emplace(currCdn, 0);

            } else if (s[i] == '#') {
                ptr = new Passage{currCdn};
            }
            tempRow.emplace_back(ptr);
        }
        lineNum++;
        theGrid.emplace_back(tempRow);
        backupGrid.emplace_back(tempRow);
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





