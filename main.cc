#include <iostream>
#include <fstream>
#include "grid.h"
using namespace std;


int main(int argc, char* argv[]) {
    /*
    ifstream inFile;
    inFile.open("defaultFloor.txt", ios::in);
    std::string s;
    while (getline(inFile, s)) {
        std::cout << s << std::endl;
    }
    */
   Grid* g = new Grid{"defaultFloor.txt"};
   delete g;




}



