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

int pythagoreanApprox(int a, int b) {
	if (a > b) {
		return a + b * 3 / 7; 
	} else {
		return b + a * 3 / 7;
	}
}


int ColorRGB(char R, char G, char B) {
	return R << 16 + G << 8 + B;
}