#include "Line.cpp"
#include <cmath>

#ifndef MISSILE
#define MISSILE
#define X_POS 'x'
#define Y_POS 'y'

using namespace std;

class Missile : public Line {
private:
	int xstart, ystart, xtarget, ytarget, length;
	int xc, yc;
	int xdir, ydir;
	const int velocityX = 2;
	const int velocityY = 2;
	int32 colors[3] = {RED, GREEN, BLUE};
	int endflag = 0;

	int currentLength() {
		int xdif = x2 - x1;
		int ydif = y2 - y1;
		return sqrt((xdif * xdif) + (ydif * ydif));
	}

	bool isOutOfBound(char type, int pos) {
		if (type == X_POS) {
			if (xdir > 0) {
				return pos >= xtarget;
			} else {
				return pos <= xtarget;
			}
		} else if (type == Y_POS) {
			if (ydir > 0) {
				return pos >= ytarget;
			} else {
				return pos <= ytarget;
			}
		} else {
			return false;
		}
	}

public:
	/**
	 * Missile will move from (xstart, ystart) to (xtarget, ytarget) with maximum length of "length"
	 */ 
	Missile(int xstart, int ystart, int xtarget, int ytarget, int length) : Line(xstart, ystart, xstart, ystart) {
		this->xstart = xstart;
		this->ystart = ystart;
		this->xtarget = xtarget;
		this->ytarget = ytarget;
		this->length = length;

		if (xstart <= xtarget) {
			xdir = 1;
		} else {
			xdir = -1;
		}
		if (ystart <= ytarget) {
			ydir = 1;
		} else {
			ydir = -1;
		}
	}

	bool update() {
		if (currentLength() >= length || endflag == 2) {
			if (!isOutOfBound(X_POS, x1) && !isOutOfBound(Y_POS, y1)) {
				this->x1 += velocityX * xdir;
				this->y1 += velocityY * ydir;
			} else {
				endflag = endflag | 1;
			}
		}

		if (!isOutOfBound(X_POS, x2) && !isOutOfBound(Y_POS, y2)) {
			this->x2 += velocityX * xdir;
			this->y2 += velocityY * ydir;
		} else {
			endflag = endflag | 2;
		}

		this->draw(3, colors);

		return endflag == 3;
	}

	/**
	 * Missile centered position is assumed to be in the end of the line (x2, y2)
	 * Get the position by calling this function
	 * 
	 * @output x
	 * @output y
	 */ 
	void getPosition(int* x, int* y) {
		*x = this->x2;
		*y = this->y2;
	}
};

#endif