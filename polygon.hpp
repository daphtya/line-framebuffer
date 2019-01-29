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

#define NULL_OBJ 0
#define PLAYER_OBJ 1
#define ENEMY_OBJ 2
#define BULLET_OBJ 3

class Polygon : public Drawable {
protected:
    std::vector<Coordinate*>* points;
    Coordinate* anchor;
    color c;
    double scaleFactor;
    double rotation; // rad
    char id; // polygon identifier

public:
    Polygon() {
        this->points = new std::vector<Coordinate*>();
        this->c = CWHITE;
        this->scaleFactor = 1;
        this->rotation = 0;
        this->id = NULL_OBJ;
    }

    Polygon(std::string filename, color c, char id) : Polygon() {
        std::ifstream f(filename);
        int x, y;
        while (f >> x >> y) {
            this->addPoint(new Coordinate(x, y));
        }
        f.close();
        this->c = c;
        this->anchor = this->getCenter();
        this->id = id;
    }

    ~Polygon() {
        for (int i = 0; i < this->points->size(); i++) {
            delete this->points->at(i);
        }
        delete this->points;
        delete this->anchor;
    }

    void moveWithoutAnchor(int dx, int dy) {
        for (int i = 0;  i < this->points->size(); i++) {
            this->points->at(i)->move(dx, dy);
        }
    }

    void addPoint(Coordinate* coordinate) {
        this->points->push_back(coordinate);
    }

    void move(int dx, int dy) {
        for (int i = 0;  i < this->points->size(); i++) {
            this->points->at(i)->move(dx, dy);
        }
        this->anchor->move(dx, dy);
    }

    void moveTo(int x, int y, int maxVelocity = -1) {
        int dx = x - this->anchor->getX();
        int dy = y - this->anchor->getY();
        if (maxVelocity >= 0 && (dx != 0 || dy != 0)) {
            double factor = maxVelocity * 1.0 / sqrt(dx * dx + dy * dy);
            dx *= factor;
            dy *= factor;
        }
        this->move(dx, dy);
    }

    void scale(double scaleFactor) {
        this->scaleFactor *= scaleFactor;
    }

    void scaleTo(double scaleFactor, double maxVelocity = 0) {
        if (this->scaleFactor > scaleFactor) {
            this->scaleFactor = std::max(this->scaleFactor - maxVelocity, scaleFactor);
        } else {
            this->scaleFactor = std::min(this->scaleFactor + maxVelocity, scaleFactor);
        }
    }

    void rotate(double rotation) {
        this->rotation += rotation;
    }

    void rotateTo(double rotation, double maxVelocity = 0) {
        if (this->rotation > rotation) {
            this->rotation = std::max(this->rotation - maxVelocity, rotation);
        } else {
            this->rotation = std::min(this->rotation + maxVelocity, rotation);
        }
    }

    double getScaleFactor() const {
        return this->scaleFactor;
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

    bool isOverlapping(std::pair<Coordinate*, Coordinate*>* otherBoundingBox) {
        std::pair<Coordinate*, Coordinate*>* thisBoundingBox = this->getBoundingBox();
        bool overlapX, overlapY;

        overlapX = otherBoundingBox->second->getX() >= thisBoundingBox->first->getX() &&
            thisBoundingBox->second->getX() >= otherBoundingBox->first->getX();

        overlapY = otherBoundingBox->second->getY() >= thisBoundingBox->first->getY() &&
            thisBoundingBox->second->getY() >= otherBoundingBox->first->getY();

        delete thisBoundingBox->first;
        delete thisBoundingBox->second;
        delete thisBoundingBox;
        return overlapX && overlapY; 
    }

    void draw(FrameBuffer* framebuffer) {
        int nLines = this->points->size();

        for (int i = 0; i < this->points->size(); i++) {
            Coordinate* c1 = this->points->at(i)->transform(this->scaleFactor, this->rotation, this->anchor);
            Coordinate* c2 = this->points->at((i + 1) % nLines)->transform(this->scaleFactor, this->rotation, this->anchor);
            Line* line = new Line(c1->getX(), c1->getY(), c2->getX(), c2->getY(), this->c, this->c);
            line->draw(framebuffer);
            delete c1;
            delete c2;
            delete line;
        }
    }
};

#endif