#include "Line.cpp"
#include "DrawnObject.cpp"

#ifndef PESAWAT
#define PESAWAT


#define PLANE_SPEED 2

class Pesawat {
private:
	DrawnObject object;
	bool facing_left;

public:
	Pesawat(int x, int y) : Pesawat(x,y,true) { }

	Pesawat(int x, int y, bool facing_left) {
		this->facing_left = facing_left;
		int tail_disp = 30 ? facing_left : -30;

		Line line1 (x,y,x+tail_disp, y+10);
		Line line2 (x,y,x+tail_disp, y-10);
		Line line3 (x+tail_disp, y+10, x+tail_disp, y-10);

		Line obj_lines[] = {line1, line2, line3};
		this->object = DrawnObject(obj_lines, 3, 3);
	}

	void draw(char *fbp) {
		this->object.draw(fbp);
	}

	void update() {
		int dx = PLANE_SPEED ? facing_left : -PLANE_SPEED;
		this->object.moveObject(dx, 0);
	}

	int* getCenter() {
		return this->object.getCenter();
	}

	void swapDirection(){
		this->facing_left = this->facing_left == false;
	}
};


#endif