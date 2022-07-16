#include <iostream>
#include <fstream>
using namespace std;


int main(int argc, char* argv[]) {
    ifstream inFile;
    inFile.open("defaultFloor.txt", ios::in);
    std::string s;
    while (getline(inFile, s)) {
        std::cout << s << std::endl;
    }



}



