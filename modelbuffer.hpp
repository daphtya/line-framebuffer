#ifndef MODEL
#define MODEL

#include "color.hpp"
#include "utils.hpp"
#include "framebuffer.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class ModelBuffer : public FrameBuffer {
private:
	color* buffer;
	long int buffersize; 
	int Xres, Yres;
	int XShift, YShift;
public:
	ModelBuffer(int Xres, int Yres, int Xshift=0, int Yshift=0) {
		this->Xres = Xres;
		this->Yres = Yres;
		this->XShift = Xshift;
		this->YShift = Yshift;
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
        int x = coordinate->getX() - this->XShift;
        int y = coordinate->getY() - this->YShift;
        if (y >= 0 && y < this->getYRes()) {
            if (x >= 0 && x < this->getXRes()) {
                long int location = x + y * this->Xres;
                this->buffer[location] = c;
            }
        }
    }

    color lazyCheck(Coordinate* coordinate) {
        int x = coordinate->getX() - this->XShift;
        int y = coordinate->getY() - this->YShift;
        if (y >= 0 && y < this->getYRes()) {
            if (x >= 0 && x < this->getXRes()) {
                long int location = x + y * this->Xres;
                return this->buffer[location];
            }
        }
    }

    void putInto(FrameBuffer* fb) {
    	for (int j = 0; j < this->Yres; j++){
    		long int location = j * this->Xres;
    		for (int i = 0; i < this->Xres; i++) {
    			if (this->buffer[location] != 0) { 				
	    			Coordinate* coor = new Coordinate(this->XShift + i, this->YShift + j);
	    			fb->lazyDraw(coor, this->buffer[location]);
	    			delete coor;
	    		}
    			location++; 
    		}
    	}
    }
};

#endif