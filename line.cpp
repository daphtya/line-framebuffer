#include "color.cpp"
#include "coordinate.cpp"
#include "framebuffer.cpp"


#ifndef LINE
#define LINE

using namespace std;

class Line {
	public:
		Coordinate* from;
		Coordinate* to;
		color from_color;
		color to_color;

	Line() {
		this->from = new Coordinate(0, 0);
		this->to = new Coordinate(0, 0);
		this->from_color = CWHITE;
		this->to_color = CWHITE;
	}

	Line(int from_x, int from_y, int to_x, int to_y) : Line() {
		this->setFrom(from_x, from_y);
		this->setTo(to_x, to_y);
	}

	Line(int from_x, int from_y, int to_x, int to_y, color from_color, color to_color) : Line(from_x, from_y, to_x, to_y) {
		this->setFrom(from_x, from_y);
		this->setTo(to_x, to_y);
		this->from_color = from_color;
		this->to_color = to_color;
	}

	~Line() {
		delete this->from;
		delete this->to;
	}

	void setFrom(int x, int y) {
		this->from->setX(x);
		this->from->setY(y);
	}

	void setFrom(int x, int y, color from_color) {
		this->from->setX(x);
		this->from->setY(y);
		this->from_color = from_color;
	}

	void setTo(int x, int y) {
		this->to->setX(x);
		this->to->setY(y);
	}

	void setTo(int x, int y, color to_color) {
		this->to->setX(x);
		this->to->setY(y);
		this->to_color = to_color;
	}

	void moveLine(int dx, int dy) {
		this->from->move(dx, dy);
		this->to->move(dx, dy);
	}

	void draw(FrameBuffer* framebuffer) {
		std::cout << "draw line from " << *(this->from) << " to " << *(this->to) << std::endl;
		int dx = this->to->getX() - this->from->getX(); int signed_dx = dx / abs(dx);
		int dy = this->to->getY() - this->from->getY(); int signed_dy = dy / abs(dy);

		int x, y, D;

		x = this->from->getX();
		y = this->from->getY();
		D = 2 * dx - dy;

		for (; y <= this->to->getY(); y += signed_dy) {
			Coordinate* coordinate = new Coordinate(x, y);
			framebuffer->lazyDraw(coordinate, CWHITE);
			delete coordinate;
			if (D > 0) {
				x += signed_dx;
				D -= 2 * dy;
			}
			D += 2 * dx;
		}
	}
};

#endif