#ifndef MODEL
#define MODEL

#include "color.hpp"
#include "utils.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class ModelBuffer {
private:
	color* buffer;
	long int buffersize; 
	int Xres, Yres;
public:
	ModelBuffer(int Xres, int Yres) {
		this->Xres = Xres;
		this->Yres = Yres;
		buffersize = Xres * Yres;
		buffer = new color[Xres*Yres];
	}	

	~ModelBuffer() {
		delete[] buffer;
	}

	int getXRes() const {return Xres;}
	int getYRes() const {return Yres;}

	void clearScreen() {
		memset(this->buffer, 0, this->buffersize);
	}

	void lazyDraw(Coordinate* coordinate, color c) {
        int x = coordinate->getX();
        int y = coordinate->getY();
        if (y >= 0 && y < this->getYRes()) {
            if (x >= 0 && x < this->getXRes()) {
                long int location = (x + this->getXOffset()) + (y + this->getYOffset()) * this->getYLength() / 4;
                this->buffer[location] = c;
            }
        }
    }

    color lazyCheck(Coordinate* coordinate) {
        int x = coordinate->getX();
        int y = coordinate->getY();
        if (y >= 0 && y < this->getYRes()) {
            if (x >= 0 && x < this->getXRes()) {
                long int location = (x + this->getXOffset()) + (y + this->getYOffset()) * this->getYLength() / 4;
                return this->buffer[location];
            }
        }
    }
};

#endif