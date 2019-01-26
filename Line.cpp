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

	void draw(char* fbp, char color[3]){
		draw(fbp, this->x2, this->y2, color);
	}

	void draw(char* fbp, int x2, int y2, char color[3]) {
		//use bresenham algorithm here
	} 

	void animateDrawing(char* fbp, int dt, int color_num, char (*colors)[3]){
		if (animationTime == 0)
			return;
		float phase = (float) timePassed / animationTime; //calculate phase
		i = 0;
		while (i < color_num) { // search for the appropriate color for this phase
			float colorphase = ((float) i + 1)/color_num;
			if (phase > colorphase)
				break;
			i++;
		}

		int tempx2 = x1 + (int)((x2-x1) * phase);
		int tempy2 = y1 + (int)((y2-y1) * phase);

		this->draw(fbp, tempx2, tempy2, colors[i]);
		timePassed += dt;
	}

};