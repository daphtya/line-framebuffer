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

	void draw(int color_num, int32 *colors, int color_interval) { //draw a line with multiple colors
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
					putpixel(x, y, color);
					y = y + 1;
					p = p + 2*dy - 2*dx;
				} else {
					putpixel(x, y, color);
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

	void draw(int32 color){
		draw(this->x1, this->y1, this->x2, this->y2, color);
	}

	void draw(int color_num, int32 *colors) { //draw a line with multiple colors
		int dx = (x2 - x1)/color_num;
		int dy = (y2 - y1)/color_num;

		int xi = x1;
		int yi = y1;
		for (int i = 0; i < color_num; i++) {
			draw(xi, yi, xi + dx, yi + dy, colors[i]);
			xi = xi + dx;
			yi = yi + dy;
		}
		draw(xi, yi, x2, y2, colors[color_num-1]);
	}

	void draw(int x0, int y0, int x1, int y1, int32 color) {
		if (abs(y1 - y0) < abs(x1 - x0)) {
			if (x0 > x1) {
				plotLineLow(x1, y1, x0, y0, color);
			} else {
				plotLineLow(x0, y0, x1, y1, color);
			}
		} else {
			if (y0 > y1) {
				plotLineHigh(x1, y1, x0, y0, color);
			} else {
				plotLineHigh(x0, y0, x1, y1, color);
			}
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
		this->draw(this->x1, this->y1, tempx2, tempy2, RED);
		timePassed += dt;
	}

	private:
		void plotLineHigh(int x0, int y0, int x1, int y1, int32 color) {
			int dx = x1 - x0;
			int dy = y1 - y0;
			int xi = 1;
			
			if (dx < 0) {
				xi = -1;
				dx = -dx;
			}
		
			int D = 2*dx - dy;
			int x = x0;

			for (int y = y0; y <= y1; y++) {
				putpixel(x , y, color);
				if (D > 0) {
					x = x + xi;
					D = D - 2*dy;
				}
				D = D + 2*dx;
			}
		}

		void plotLineLow(int x0, int y0, int x1, int y1, int32 color) {
			int dx = x1 - x0;
			int dy = y1 - y0;
			int yi = 1;

			if (dy < 0) {
				yi = -1;
				dy = -dy;
			}
			
			int D = 2*dy - dx;
			int y = y0;

			for (int x = x0; x <= x1; x++) {
				putpixel(x, y, color);
				if (D > 0) {
					y = y + yi;
					D = D - 2*dx;
				}
				D = D + 2*dy;
			}
		}
};

#endif