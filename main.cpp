#include <curses.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "utils.hpp"

#include "animated.hpp"
#include "drawable.hpp"
#include "framebuffer.hpp"
#include "line.hpp"
#include "polygon.hpp"

#define COMMAND_QUIT ' '
#define COMMAND_ROTATE_LEFT 'q'
#define COMMAND_ROTATE_RIGHT 'e'
#define COMMAND_MOVE_LEFT 'a'
#define COMMAND_MOVE_RIGHT 'd'
#define COMMAND_SHOOT 'w'

void readInput(FrameBuffer* framebuffer, std::vector<Drawable*>* objects, bool* run) {
    initscr();
    timeout(-1);
    while (*run) {
        char c = getch();
        if (c == COMMAND_QUIT) {
            *run = false;
        } else if (c == COMMAND_ROTATE_LEFT) {
            Polygon* player = (Polygon*) objects->at(0);
            if (player->getRotation() > -1.2) { player->rotate(-0.2); }
        } else if (c == COMMAND_ROTATE_RIGHT) {
            Polygon* player = (Polygon*) objects->at(0);
            if (player->getRotation() < 1.2) { player->rotate(0.2); }
        } else if (c == COMMAND_MOVE_LEFT) {
            Polygon* player = (Polygon*) objects->at(0);
            player->move(-3, 0);
        } else if (c == COMMAND_MOVE_RIGHT) {
            Polygon* player = (Polygon*) objects->at(0);
            player->move(3, 0);
        } else if (c == COMMAND_SHOOT) {
            Polygon* player = (Polygon*) objects->at(0);
            std::pair<Coordinate*, Coordinate*>* boundingBox = player->getBoundingBox();
            Coordinate* top = new Coordinate((boundingBox->first->getX() + boundingBox->second->getX()) / 2, boundingBox->first->getY());
            int toX = top->getX() + 1000 * tan(player->getRotation());
            Coordinate* dest = new Coordinate(toX, top->getY() - 1000);

            Animated* laser = new Animated("images/laser.point", CRED, BULLET_OBJ, false, 5, 0.1, 1);
            laser->addAnchorKeyframe(new Coordinate(top->getX(), top->getY()));
            laser->addAnchorKeyframe(dest);
            laser->addScaleKeyframe(1);
            laser->addScaleKeyframe(10);
            laser->addRotationKeyframe(player->getRotation());
            // laser->addRotationKeyframe(720);
            laser->moveWithoutAnchor(0, -10);

            objects->push_back(laser);

            delete top;
            delete boundingBox->first;
            delete boundingBox->second;
            delete boundingBox;
        }
    }
    endwin();
}

void draw(FrameBuffer* framebuffer, std::vector<Drawable*>* objects, bool* run) {
    char type;

    while (*run) {
        framebuffer->clearScreen();

        for (int i = 0; i < objects->size(); i++) {
            objects->at(i)->draw(framebuffer);
            objects->at(i)->animate();
        }
        framebuffer->draw();
        usleep(10000);
    }
}

void enemyUpdate(FrameBuffer* framebuffer, std::vector<Drawable*>* objects, Coordinate* coor) {
    int threshold = 50;
    Polygon* enemy = (Polygon*) objects->at(1);;
    Polygon* obj;
    bool hit = false;

    srand(time(0));
    while (!hit) {
        //update next dest when enemy has arrived to previous destination
        if (abs(enemy->getCenter()->getX() - coor->getX()) < threshold) {
            coor->setX(rand() % framebuffer->getXRes()*0.9 + framebuffer->getXRes()*0.05);
            coor->setY(rand() % framebuffer->getYRes()*0.2 + framebuffer->getYRes()*0.05);
        }

        //collision detection
        for (int i = 0; i < objects->size(); i++) {
            obj = (Polygon*) objects->at(i);
            if (obj->getId() == BULLET_OBJ) {
                if (enemy->isOverlapping(obj->getBoundingBox())) {
                    objects->erase(objects->begin()+1);
                    objects->erase(objects->begin()+i);
                    hit = true;
                }
            }
        }
    }
}

int main(int argc, char **args) {
    FrameBuffer* framebuffer;
    try {
        framebuffer = new FrameBuffer();
    } catch (Exception e) {
        e.print();
        return 1;
    }

    bool run = true;
    std::vector<Drawable*>* objects = new std::vector<Drawable*>;

    Polygon* player = new Polygon("images/pesawat.point", CBLUE, PLAYER_OBJ);
    player->moveTo(framebuffer->getXRes() / 2, framebuffer->getYRes() - 40);
    player->scale(4);
    objects->push_back(player);

    Coordinate* coor = new Coordinate(framebuffer->getXRes() / 2, framebuffer->getYRes()*0.1);
    Animated* enemy = new Animated("images/ufo.point", CMAGENTA, ENEMY_OBJ, coor);
    enemy->moveTo(framebuffer->getXRes() / 2, framebuffer->getYRes()*0.1);
    enemy->scale(4);
    objects->push_back(enemy);

    std::thread* t0 = new std::thread(readInput, framebuffer, objects, &run);
    std::thread* t1 = new std::thread(draw, framebuffer, objects, &run);
    std::thread* t2 = new std::thread(enemyUpdate, framebuffer, objects, coor);

    t0->join();
    t1->join();
    t2->join();

    delete player;
    delete t0;
    delete t1;
    delete framebuffer;

	return 0;
}