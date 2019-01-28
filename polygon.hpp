#ifndef POLYGON
#define POLYGON

#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include "coordinate.hpp"
#include "drawable.hpp"
#include "framebuffer.hpp"
#include "line.hpp"
#include "utils.hpp"

class Polygon : public Drawable {
private:
    std::vector<Coordinate*>* points;
    color c;
    double scaleFactor;
    double rotation; // rad

public:
    Polygon() {
        this->points = new std::vector<Coordinate*>();
        this->c = CWHITE;
        this->scaleFactor = 1;
        this->rotation = 0;
    }

    Polygon(std::string filename, color c) : Polygon() {
        std::ifstream f(filename);
        int x, y;
        while (f >> x >> y) {
            this->addPoint(new Coordinate(x, y));
        }
        f.close();
    }

    Polygon(color c) : Polygon() {
        this->c = c;
    }

    ~Polygon() {
        for (int i = 0; i < this->points->size(); i++) {
            delete this->points->at(i);
        }
        delete this->points;
    }

    void addPoint(Coordinate* coordinate) {
        this->points->push_back(coordinate);
    }

    void move(int dx, int dy) {
        for (int i = 0;  i < this->points->size(); i++) {
            this->points->at(i)->move(dx, dy);
        }
    }

    void moveTo(int x, int y) {
        Coordinate* center = this->getCenter();
        int dx = x - center->getX();
        int dy = y - center->getY();
        delete center;
        this->move(dx, dy);
    }

    void scale(double scaleFactor) {
        this->scaleFactor *= scaleFactor;
    }

    void rotate(double rotation) {
        this->rotation += rotation;
    }

    double getRotation() const {
        return this->rotation;
    }

    std::pair<Coordinate*, Coordinate*>* getBoundingBox() {
        int minX = this->points->at(0)->getX(); int maxX = minX;
        int minY = this->points->at(0)->getY(); int maxY = minY;
        for (int i = 1; i < this->points->size(); i++) {
            int x = this->points->at(i)->getX();
            int y = this->points->at(i)->getY();
            minX = std::min(minX, x); maxX = std::max(maxX, x);
            minY = std::min(minY, y); maxY = std::max(maxY, y);
        }
        return new std::pair<Coordinate*, Coordinate*>(new Coordinate(minX, minY), new Coordinate(maxX, maxY));
    }

    Coordinate* getCenter() {
        std::pair<Coordinate*, Coordinate*>* boundingBox = this->getBoundingBox();
        int x = (boundingBox->first->getX() + boundingBox->second->getX()) / 2;
        int y = (boundingBox->first->getY() + boundingBox->second->getY()) / 2;
        delete boundingBox->first;
        delete boundingBox->second;
        delete boundingBox;
        return new Coordinate(x, y);
    }

    void draw(FrameBuffer* framebuffer) {
        int nLines = this->points->size();
        Coordinate* center = this->getCenter();

        for (int i = 0; i < this->points->size(); i++) {
            Coordinate* c1 = this->points->at(i)->transform(this->scaleFactor, this->rotation, center);
            Coordinate* c2 = this->points->at((i + 1) % nLines)->transform(this->scaleFactor, this->rotation, center);
            Line* line = new Line(c1->getX(), c1->getY(), c2->getX(), c2->getY(), this->c, this->c);
            line->draw(framebuffer);
            delete line;
        }
        delete center;
    }
};

#endif