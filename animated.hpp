#ifndef ANIMATED
#define ANIMATED

#include "polygon.hpp"
#include "coordinate.hpp"

class Animated : public Polygon {
protected:
    Coordinate* destination;

public:
    Animated(std::string filename, color c, Coordinate* destination) : Polygon(filename, c) {
        this->destination = destination;
    }

    ~Animated() {
        delete this->destination;
    }

    void animate(int velocity) {
        this->moveTo(this->destination->getX(), this->destination->getY(), velocity);
    }

    virtual bool isAnimated() const {
        return true;
    }
};

#endif