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


		//Line obj_lines[] = {pline1, pline2, pline3};
		this->object = obj;
	}

	void draw(int32 color) {
		this->object.draw(color);
	}

	void update() {
		int dx = this->facing_left ? -PLANE_SPEED : PLANE_SPEED;
		this->object.moveObject(dx, 0);
	}

	int* getCenter(int *x, int *y) {
		this->object.getCenter(x,y);
	}

	void swapDirection(){
		this->facing_left = this->facing_left ? false : true;
		int tail_disp = facing_left ? 30 : -30;
		int x, y;
		getCenter(&x, &y);

		DrawnObject obj(3);
		Line pline1 (x,y,x+tail_disp, y+10);
		Line pline2 (x,y,x+tail_disp, y-10);
		Line pline3 (x+tail_disp, y+10, x+tail_disp, y-10);
		obj.addLine(pline1);
		obj.addLine(pline2);
		obj.addLine(pline3);

		//Line obj_lines[] = {pline1, pline2, pline3};
		this->object = obj;
	}
};


#endif