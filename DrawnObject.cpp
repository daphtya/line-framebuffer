#include <vector>
#include <iostream>
#include "Line.cpp"

#ifndef DOBJECT
#define DOBJECT

using namespace std;

class DrawnObject {
private:
	Line *lines; 
	int max_line_number;
	int line_number;

public:
	DrawnObject() : DrawnObject(10) { }

	DrawnObject(int max_line_number) {
		this->max_line_number = max_line_number;
		this->line_number = 0;
		this->lines = new Line[max_line_number];
	}

	DrawnObject(Line lines[], int line_number, int max_line_number) : DrawnObject(max_line_number) {
		for (int i=0; i < line_number; i++){
			this->addLine(lines[i]);
		}
	}

	void addLine(Line line){
		if (this->line_number == this->max_line_number)
			return;
		
		lines[line_number] = line;	
		line_number++;
	}

	void moveObject(int dx, int dy) {
		for (int i = 0; i < line_number; i++) {
			lines[i].moveLine(dx, dy);
		}
	}

	void getCenter(int *x, int *y) {
		int sumx = 0;
		int sumy = 0;
		int point_num = 2*line_number;
		for (int i = 0; i < line_number; i++) {
			sumx += lines[i].x1 + lines[i].x2;
			sumy += lines[i].y1 + lines[i].y2;
		}
		*x = sumx/point_num;
		*y = sumy/point_num;
	}

	void draw(int32 color) {
		for (int i = 0; i < line_number; i++){
			lines[i].draw(color);
		}
	}
};


#endif