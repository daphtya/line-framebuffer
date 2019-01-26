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

	Line(){
		this->x1 = 0;
		this->x2 = 0;
		this->y1 = 0;
		this->y2 = 0;
	}

	Line(int x1, int y1, int x2, int y2){
		this->setLine(x1,y1,x2,y2);
		this->animationTime = 0;
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

	void draw(char* fbp){

	}

	void animateDrawing(char* fbp, int dt){

	}


};