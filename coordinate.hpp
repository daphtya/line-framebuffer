#ifndef COORDINATE
#define COORDINATE

#include <iostream>

class Coordinate {
private:
    int x;
    int y;

public:
    Coordinate() : Coordinate(0, 0) { }
    Coordinate(int x, int y) {
        this->x = x;
        this->y = y;
    }
    Coordinate(const Coordinate& others) {
        this->x = others.getX();
        this->y = others.getY();
    }

    int getX() const { return x; }
    int getY() const { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }

    void move(int dx, int dy) { this->x += dx; this->y += dy; }

    friend std::ostream &operator<<(std::ostream &output, const Coordinate& coordinate) {
        output << "(" << coordinate.getX() << ", " << coordinate.getY() << ")";
        return output;
    }
};

#endif