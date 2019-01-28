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

	Pesawat(int x, int y, bool facing_left) : object(3) {
		this->facing_left = facing_left;
		int tail_disp = facing_left ? 30 : -30;

		DrawnObject obj(3);
		Line pline1 (x,y,x+tail_disp, y+10);
		Line pline2 (x,y,x+tail_disp, y-10);
		Line pline3 (x+tail_disp, y+10, x+tail_disp, y-10);
		obj.addLine(pline1);
		obj.addLine(pline2);
		obj.addLine(pline3);


		Line obj_lines[] = {pline1, pline2, pline3};
		this->object = obj;
	}

	void draw() {
		this->object.draw();
	}

	void update() {
		int dx = this->facing_left ? -PLANE_SPEED : PLANE_SPEED;
		this->object.moveObject(dx, 0);
	}

	int* getCenter() {
		return this->object.getCenter();
	}

	void swapDirection(){
		this->facing_left = this->facing_left ? false : true;
	}
};


#endif