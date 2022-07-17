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
        std::vector<Cell*> tempRow1;
        std::vector<Cell*> tempRow2;
        int len = s.length();
        for (int i = 0; i < len; i++) {
            Coordinate currCdn{lineNum, i};
            Cell* ptr1;
            Cell* ptr2;
            if (s[i] == '|' || s[i] == '-' || s[i] == ' ') {
                ptr1 = new Wall{currCdn};
                ptr2 = new Wall{currCdn};
            } else if (s[i] == '.') {
                ptr1 = new Floor{currCdn};
                ptr2 = new Floor{currCdn};
                Coordinate currCdn{lineNum, i};
                allFloors.emplace(currCdn, 0);

            } else if (s[i] == '#') {
                ptr1 = new Passage{currCdn};
                ptr2 = new Passage{currCdn};
            }
            tempRow1.emplace_back(ptr1);
            tempRow2.emplace_back(ptr2);
        }
        lineNum++;
        theGrid.emplace_back(tempRow1);
        backupGrid.emplace_back(tempRow2);
    }
}
























Grid::~Grid() {
    int height = theGrid.size();
    std::cout << height << std::endl;
    for (int i = 0; i < height; i++) {
        int width = theGrid[i].size();
        std::cout << width << std::endl;
        for (int j; j < width; j++) {
            delete theGrid[i][j];
        }
        theGrid[i].clear();
    }
    theGrid.clear();

    height = backupGrid.size();
    for (int i = 0; i < height; i++) {
        int width = backupGrid[i].size();
        for (int j; j < width; j++) {
            delete backupGrid[i][j];
        }
        backupGrid[i].clear();
    }
    backupGrid.clear();
}





