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
#include "textdisplay.h"

Grid::Grid(std::string fileName) {
    std::ifstream ifs;
    ifs.open(fileName, std::ios::in);
    std::string s;
    int lineNum = 0;
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
            } if (s[i] == '-') {
                ptr1 = new Wall{currCdn, 2};
                ptr2 = new Wall{currCdn, 2};
            } if (s[i] == ' ') {
                ptr1 = new Wall{currCdn, 3};
                ptr2 = new Wall{currCdn, 3};
            } else if (s[i] == '.') {
                ptr1 = new Floor{currCdn};
                ptr2 = new Floor{currCdn};
                Coordinate currCdn{lineNum, i};
                allFloors.emplace(currCdn, 0);

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
        backupGrid.emplace_back(tempRow2);
    }
    td = new TextDisplay{theGrid};
    // Debugger
    std::cout << *td;
}




Grid::~Grid() {
    int height = theGrid.size();
    for (int i = 0; i < height; i++) {
        int width = theGrid[i].size();
        for (int j; j < width; j++) {
            delete theGrid[i][j];
            delete backupGrid[i][j];
        }
        theGrid[i].clear();
        backupGrid[i].clear();
    }
    theGrid.clear();
    backupGrid.clear();
    allFloors.clear();
}





