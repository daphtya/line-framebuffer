#include <vector>
#include <iostream>
#include "Line.cpp"

using namespace std;

class DrawnObject {
private:
	Line *lines; 
	int max_line_number;
	int line_number;

public:
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

	void draw(char* fbp) {
		for (int i = 0; i < line_number; i++){
			lines[i].draw(fbp);
		}
	}
};