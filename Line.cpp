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
#include "utilities.h"


#ifndef LINE
#define LINE

using namespace std;

class Line {
	public:
		int x1,y1,x2,y2;
		int animationTime;
		int timePassed;

	Line(){
		this->x1 = 0;
		this->x2 = 0;
		this->y1 = 0;
		this->y2 = 0;
		this->animationTime = 0;
		this->timePassed = 0;
	}

	Line(int x1, int y1, int x2, int y2){
		this->setLine(x1,y1,x2,y2);
	}

	Line(int x1, int y1, int x2, int y2, int animationTime) : Line(x1,y1,x2,y2) {
		this->animationTime = animationTime;
	}

	void setLine(int x1, int y1, int x2, int y2){
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}

	void moveLine(int dx, int dy) {
		this->x1 += dx;
		this->y1 += dy;
		this->x2 += dx;
		this->y2 += dy;
	}

	void draw(char* fbp, int color_num, int32 *colors, int color_interval) { //draw a line with multiple colors
		/*int tempx1 = this->x1;
		int tempy1 = this->y1
		int tempx2, tempy2;
		float colorphase = (float) //*/
		int dx, dy, p, x, y;
 		int color_counter = 0;
 		int color_index = 0;
 		int32 color = colors[color_index]; //temporary
		dx = x2 - x1;
		dy = y2 - y1;
	
		x = x1;
		y = y1;
	
		if (abs(dx) > abs(dy)) {		
			p = 2*dy - dx;
			while(x < x2) {
				if(p >= 0) {
					putpixel(fbp, x, y, color);
					y = y + 1;
					p = p + 2*dy - 2*dx;
				} else {
					putpixel(fbp, x, y, color);
					p = p + 2*dy;
				}
				x = x + 1;
				color_counter++;
				if (color_counter == color_interval) {
					color_index++;
					color = colors[color_index];
					color_counter = 0;
				}
			}
		} else {
			// if gradient > 1
		}
	}

	void draw(char* fbp, int32 color){
		draw(fbp, this->x1, this->y1, this->x2, this->y2, color);
	}

	void draw(char* fbp, int x1, int y1, int x2, int y2, int32 color) {
		int dx, dy, p, x, y;
 
		dx = x2 - x1;
		dy = y2 - y1;
	
		x = x1;
		y = y1;
	
		p = 2*dy - dx;
	
		while(x < x2) {
			if(p >= 0) {
				putpixel(fbp, x, y, color);
				y = y + 1;
				p = p + 2*dy - 2*dx;
			} else {
				putpixel(fbp, x, y, color);
				p = p + 2*dy;
			}
			x = x + 1;
		}
	} 

	void animateDrawing(char* fbp, int dt, int color_num, int32 *colors){
		if (animationTime == 0)
			return;
		float phase = (float) timePassed / animationTime; //calculate phase
		int i = 0;
		while (i < color_num) { // search for the appropriate color for this phase
			float colorphase = ((float) i + 1)/color_num;
			if (phase > colorphase)
				break;
			i++;
		}

		int tempx2 = x1 + (int)((x2-x1) * phase + 0.5); //approximate pixel position in integer
		int tempy2 = y1 + (int)((y2-y1) * phase + 0.5);

		// this->draw(fbp, tempx2, tempy2, colors[i]);
		this->draw(fbp, this->x1, this->y1, tempx2, tempy2, RED);
		timePassed += dt;
	}

	private:
		void plotlineHigh(int x1, int y1, int x2, int y2) {

		}

		void plotlineLow(int x1, int y1, int x2, int y2) {

		}
};

#endif