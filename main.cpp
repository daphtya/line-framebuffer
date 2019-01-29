#include <curses.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>

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

            Animated* laser = new Animated("images/laser.point", CRED, dest);
            laser->moveTo(top->getX(), top->getY());
            laser->moveWithoutAnchor(0, -10);
            laser->scale(4);
            laser->rotate(player->getRotation());

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
    while (*run) {
        framebuffer->clearScreen();

        for (int i = 0; i < objects->size(); i++) {
            objects->at(i)->draw(framebuffer);
            if (objects->at(i)->isAnimated()) {
                objects->at(i)->animate(10);
            }
        }
        framebuffer->draw();
        usleep(10000);
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

    Polygon* player = new Polygon("images/pesawat.point", CBLUE);
    player->moveTo(framebuffer->getXRes() / 2, framebuffer->getYRes() - 40);
    player->scale(4);
    objects->push_back(player);

    Polygon* enemy = new Polygon("images/ufo.point", CMAGENTA);
    enemy->moveTo(framebuffer->getXRes() / 2, framebuffer->getYRes()*0.1);
    enemy->scale(4);
    objects->push_back(enemy);

    std::thread* t0 = new std::thread(readInput, framebuffer, objects, &run);
    std::thread* t1 = new std::thread(draw, framebuffer, objects, &run);

    t0->join();
    t1->join();

	return 0;
}