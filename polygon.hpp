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
#include "fillable.hpp"
#include "edgebucket.hpp"

#define NULL_OBJ 1
#define PLAYER_OBJ 2
#define ENEMY_OBJ 3
#define LASER_OBJ 4
#define EXPLOSION_OBJ 5
#define PHYSICAL_OBJ 6

class Polygon : public Drawable, Fillable {
protected:
    std::vector<Coordinate*>* points;
    Coordinate* anchor;
    color c;
    double scaleFactor;
    double rotation; // rad
    char id; // polygon identifier
    edgebucket* EdgeTable = NULL;

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
        int x, y, minX, maxX, minY, maxY;
        bool first = true;
        while (f >> x >> y) {
            if (first) { minX = maxX = x; minY = maxY = y; }
            first = false;
            minX = std::min(minX, x); maxX = std::max(maxX, x);
            minY = std::min(minY, y); maxY = std::max(maxY, y);
            this->addPoint(new Coordinate(x, y));
        }
        f.close();
        this->c = c;
        this->anchor = new Coordinate((minX + maxX) / 2, (minY + maxY) / 2);
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

    Coordinate* getAnchor() const {
        return this->anchor;
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
        int dist2 = dx * dx + dy * dy;
        if (maxVelocity >= 0 && dist2 > maxVelocity && (dx != 0 || dy != 0)) {
            double factor = maxVelocity * 1.0 / sqrt(dist2);
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

    char getId() const {
        return this->id;
    }

    std::pair<Coordinate*, Coordinate*>* getBoundingBox() {
        Coordinate* point = this->points->at(0)->transform(this->scaleFactor, this->rotation, this->anchor);
        int minX = point->getX(); int maxX = minX;
        int minY = point->getY(); int maxY = minY;
        delete point;
        for (int i = 1; i < this->points->size(); i++) {
            point = this->points->at(i)->transform(this->scaleFactor, this->rotation, this->anchor);
            int x = point->getX();
            int y = point->getY();
            minX = std::min(minX, x); maxX = std::max(maxX, x);
            minY = std::min(minY, y); maxY = std::max(maxY, y);
            delete point;
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

        fill(framebuffer);
    }

    void createEdges(){
        this->EdgeTable = new edgebucket[this->points->size()];
        int nLines = this->points->size();

        for (int i = 0; i < this->points->size(); i++) {
            Coordinate* c1 = this->points->at(i)->transform(this->scaleFactor, this->rotation, this->anchor);
            Coordinate* c2 = this->points->at((i + 1) % nLines)->transform(this->scaleFactor, this->rotation, this->anchor);
            int dx = c2->getX() - c1->getX();
            int dy = c2->getY() - c1->getY();

            if(dy != 0){
                int yMax, yMin, x, sign;
                if (c1->getY() > c2->getY()){
                    yMax = c1->getY();
                    yMin = c2->getY();
                    x = c1 ->getX();
                }else{
                    yMax = c2->getY();
                    yMin = c1->getY();
                    x = c1 ->getX();
                }

                

                if(dx != 0){
                    if(dy/dx > 0){
                        sign = 1;
                    }else{
                        sign = -1;
                    }
                }else{
                    sign = 0;
                }

                this->EdgeTable[i] = edgebucket(yMax, yMin, x, sign, dx, dy, 0);
                //std::cout << "yMax " << yMax << " yMin " << yMin << " x " << x << " sign " << sign << " dx " << dx << " dy " << dy << std::endl;
            }else{
                this->EdgeTable[i] = edgebucket(0, 0, 0, 0, 0, 0, 0);
            }


            delete c1;
            delete c2;
        }
    }

    void sortEdges(){
        //sorts edges by starting X;
        int nLines = this->points->size();
        edgebucket e;
        int MinX;
        int MinIndex;

        for (int i = 0; i < nLines; i++){
            MinIndex = i;
            MinX = EdgeTable[i].x;
            for(int j = i; j<nLines; j++){
                if(EdgeTable[j].x < MinX){
                    MinX = EdgeTable[j].x;
                    MinIndex = j;
                }
            }

            if (MinIndex != i){
                e = EdgeTable[i];
                EdgeTable[i] = EdgeTable[MinIndex];
                EdgeTable[MinIndex] = e;
            }
        }
    }

    void fill(FrameBuffer* framebuffer) {
        this->createEdges();
        int minY, maxY;
        int nLines = this->points->size();
        bool activeList[nLines];
        minY = this->points->at(0)->transform(this->scaleFactor, this->rotation, this->anchor)->getY();
        maxY = minY;

        for (int i = 0; i < nLines; i++) {
            Coordinate* coor = this->points->at(i)->transform(this->scaleFactor, this->rotation, this->anchor);
            if (coor->getY() < minY){
                minY = coor->getY();
            }else if (coor->getY() > maxY){
                maxY = coor->getY();
            }

            activeList[i] = false;

            delete coor;
        }

        for(int y = minY; y <= maxY; y++){
            //Add and remove edge from active list
            this->sortEdges();
            for (int i = 0; i < nLines; i++){
                if (EdgeTable[i].MaxY >= y){
                    activeList[i] == false;
                }else if(EdgeTable[i].MinY <= y){
                    activeList[i] = true;
                } 
            }

            Coordinate* c1;
            Coordinate* c2;
            bool odd = false;
            for (int i = 0; i < nLines; i++){
                if(activeList[i]){
                    Coordinate* coor = new Coordinate(EdgeTable[i].x, y - 1);
                    color col1, col2, col3;
                    col1 = framebuffer->lazyCheck(coor);
                    coor->setX(coor->getX() + 1);
                    col2 = framebuffer->lazyCheck(coor);
                    coor->setX(coor->getX() - 2);
                    col3 = framebuffer->lazyCheck(coor);
                    delete coor;
                    if(col1 == this->c || col2 == this->c || col3 == this->c){
                        if(!odd){
                            odd = true;
                            c1 = new Coordinate(EdgeTable[i].x, y);
                        }else{
                            odd = false;
                            c2 = new Coordinate(EdgeTable[i].x, y);

                            Line* line = new Line(c1->getX(), c1->getY(), c2->getX(), c2->getY(), this->c, this->c);
                            line->draw(framebuffer);
                            delete c1;
                            delete c2;
                            delete line;
                        }

                        if(EdgeTable[i].dX != 0){
                            EdgeTable[i].sum += EdgeTable[i].dX;
                        }

                        while(EdgeTable[i].sum >= EdgeTable[i].dY){
                            EdgeTable[i].x += EdgeTable[i].sign;
                            EdgeTable[i].sum -= EdgeTable[i].dY;
                        }
                    }
                }
            }
        }

    }
};

#endif