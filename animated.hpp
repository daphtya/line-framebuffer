#ifndef ANIMATED
#define ANIMATED

#include <vector>

#include "polygon.hpp"
#include "coordinate.hpp"

class Animated : public Polygon {
protected:
    bool loop;

    int maxAnchorVelocity;
    int nextAnchorKeyframes;
    std::vector<Coordinate*>* anchorKeyframes;

    double maxScaleVelocity;
    int nextScaleKeyframes;
    std::vector<double>* scaleKeyframes;

public:
    Animated(std::string filename, color c, bool loop = false, int maxAnchorVelocity = 0, double maxScaleVelocity = 0) : Polygon(filename, c) {
        this->loop = loop;

        this->maxAnchorVelocity = maxAnchorVelocity;
        this->nextAnchorKeyframes = 0;
        this->anchorKeyframes = new std::vector<Coordinate*>();

        this->maxScaleVelocity = maxScaleVelocity;
        this->nextScaleKeyframes = 0;
        this->scaleKeyframes = new std::vector<double>();
    }

    ~Animated() {
        for (int i = 0; i < this->anchorKeyframes->size(); i++) {
            delete this->anchorKeyframes->at(i);
        }
        delete this->anchorKeyframes;
    }

    void addAnchorKeyframe(Coordinate* anchor) {
        if (this->anchorKeyframes->empty()) {
            this->moveTo(anchor->getX(), anchor->getY());
            this->nextAnchorKeyframes = 1;
        }
        this->anchorKeyframes->push_back(anchor);
    }

    void addScaleKeyframe(double scale) {
        if (this->scaleKeyframes->empty()) {
            this->scale(scale);
            this->nextScaleKeyframes = 1;
        }
        this->scaleKeyframes->push_back(scale);
    }

    void animate() {
        if (!this->anchorKeyframes->empty() && this->anchorKeyframes->size() > this->nextAnchorKeyframes) {
            int fromX = this->anchor->getX();
            int fromY = this->anchor->getY();
            Coordinate* dest = this->anchorKeyframes->at(this->nextAnchorKeyframes);
            this->moveTo(dest->getX(), dest->getY(), this->maxAnchorVelocity);
            if (this->anchor->getX() == fromX && this->anchor->getY() == fromY) {
                this->nextAnchorKeyframes++;
            }
        }

        if (!this->scaleKeyframes->empty() && this->scaleKeyframes->size() > this->nextScaleKeyframes) {
            double from = this->getScaleFactor();
            double to = this->scaleKeyframes->at(this->nextScaleKeyframes);
            flog(from);
            flog(to);
            this->scaleTo(to, this->maxScaleVelocity);
            if (from == this->getScaleFactor()) {
                this->nextScaleKeyframes++;
            }
        }
    }
};

#endif