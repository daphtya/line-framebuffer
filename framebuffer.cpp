#ifndef FRAMEBUFFER
#define FRAMEBUFFER

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "color.cpp"
#include "exception.cpp"
#include "coordinate.cpp"

class FrameBuffer {
private:
    color* fbp;
    color** lazy;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize;

public:
    FrameBuffer() {
	    int fbfd = 0;

        fbfd = open("/dev/fb0", O_RDWR);
        if (fbfd == -1) {
            throw new Exception("Error: cannot open framebuffer device");
        }
        std::cout << "The framebuffer device was opened successfully." << std::endl;

        if (ioctl(fbfd, FBIOGET_FSCREENINFO, &(this->finfo)) == -1) {
            throw new Exception("Error reading fixed information");
        }

        if (ioctl(fbfd, FBIOGET_VSCREENINFO, &(this->vinfo)) == -1) {
            throw new Exception("Error reading variable information");
        }

        this->screensize = this->vinfo.xres * this->vinfo.yres * this->vinfo.bits_per_pixel / 8;

        std::cout << "Screen size = " << this->getXRes() << " x " << this->getYRes() << std::endl;
        std::cout << "Offset = " << this->getXOffset() << ", " << this->getYOffset() << std::endl;
        std::cout << "Length = " << this->getXLength() << ", " << this->getYLength() << std::endl;
        std::cout << "Color " << this->vinfo.bits_per_pixel << "bpp" << std::endl;
        std::cout << "Screensize " << this->screensize << std::endl;

        this->fbp = (color *) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
        if (this->fbp == (color*) -1) {
            throw new Exception("Error: failed to map framebuffer device to memory");
        }
        this->lazy = (color**) malloc(this->getXRes() * sizeof(color*));
        for (int i=0; i<this->getXRes(); i++) {
            this->lazy[i] = (color*) malloc(this->getYRes() * sizeof(color));
            for (int j=0; j<this->getYRes(); j++) {
                this->lazy[i][j] = 0;
            }
        }

        std::cout << "The framebuffer device was mapped to memory successfully." << std::endl;
    }

    unsigned int getXRes() const { return this->vinfo.xres; }
    unsigned int getYRes() const { return this->vinfo.yres; }
    unsigned int getXOffset() const { return this->vinfo.xoffset; }
    unsigned int getYOffset() const { return this->vinfo.yoffset; }
    unsigned int getXLength() const { return this->vinfo.bits_per_pixel; }
    unsigned int getYLength() const { return this->finfo.line_length; }

    void lazyDraw(Coordinate* coordinate, color c) {
        int x = coordinate->getX();
        int y = coordinate->getY();
        if (y >= 0 && y < this->getYRes()) {
            if (x >= 0 && x < this->getXRes()) {
                this->lazy[x][y] = c;
            }
        }
    }

    void draw() {
        memset(this->fbp, 0, this->screensize);
        for (int y = 0; y < this->getYRes() - 6; y++) {
            for (int x = 0; x < this->getXRes() - 6; x++) {
                long int location = (x + this->getXOffset()) + (y + this->getYOffset()) * this->getYLength() / 4;
                this->fbp[location] = this->lazy[x][y];
            }
        }
    }
};

#endif