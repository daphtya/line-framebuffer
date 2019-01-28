#include <fstream>
#include <iostream>

#include "utils.hpp"

void log(std::string s) {
    std::ofstream fout("out.log", std::ios::out | std::ios::app);
    fout << s << std::endl;
    fout.close();
}