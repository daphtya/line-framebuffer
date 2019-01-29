#include <fstream>
#include <iostream>

#include "utils.hpp"

void flog(std::string s) {
    std::ofstream fout("out.log", std::ios::out | std::ios::app);
    fout << s << std::endl;
    fout.close();
}

void flog(double f) {
    std::ofstream fout("out.log", std::ios::out | std::ios::app);
    fout << f << std::endl;
    fout.close();
}

void flog(int x) {
    std::ofstream fout("out.log", std::ios::out | std::ios::app);
    fout << x << std::endl;
    fout.close();
}
