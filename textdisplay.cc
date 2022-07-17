#include <vector>
#include <iostream>
#include "textdisplay.h"
#include "cell.h"
#include "subject.h"
#include "coordinate.h"

TextDisplay::TextDisplay(std::vector<std::vector<Cell*>>& g) {
    int height = g.size();
    for (int i = 0; i < height; i++) {
        int width = g[i].size();
        std::vector<char> tempRow;
        for (int j = 0; j < width; j++) {
            std::string name = g[i][j]->getName();
            if (name == "Wall") {
                int walltype = g[i][j]->state();
                if (walltype == 1) {
                    tempRow.emplace_back('|');
                } else if (walltype == 2) {
                    tempRow.emplace_back('-');
                } else if (walltype == 3) {
                    tempRow.emplace_back(' ');
                }
            } else if (name == "Passage") {
                int passagetype = g[i][j]->state();
                if (passagetype == 1) {
                    tempRow.emplace_back('#');
                } else if (passagetype == 2) {
                    tempRow.emplace_back('+');
                }
            } else if (name == "Floor") {
                tempRow.emplace_back('.');
            }
        }
        grid.emplace_back(tempRow);
    }
}

void TextDisplay::notify(Subject& whoNotified) {
    Coordinate cdn = whoNotified.getState().first;
    char updatedChar = whoNotified.getState().second;
    grid[cdn.x][cdn.y] = updatedChar;
}

std::ostream& operator<<(std::ostream& out, TextDisplay& td) {
    int height = td.grid.size();
    for (int i = 0; i < height; i++) {
        int width = td.grid[i].size();
        for (int j = 0; j < width; j++) {
            out << td.grid[i][j];
        }
        out << std::endl;
    }
    return out;
}
