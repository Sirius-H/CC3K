#ifndef TERMCODES_H 
#define TERMCODES_H 
#include <string>

// Use these for the colour version
const std::string BLACK = "\033[30m";
const std::string RED =  "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW =  "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";


// Use these for the monochrome version
const std::string UNDERLINE = "\033[4m";
const std::string REVERSE = "\033[7m";

// Use this for both versions
const std::string RESET =  "\033[0m";
#endif
