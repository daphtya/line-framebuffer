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
            Coordinate* top = new Coordinate(player->getAnchor()->getX(), player->getAnchor()->getY());
            int toX = top->getX() + 1000 * tan(player->getRotation());

            Animated* laser = new Animated("images/laser.point", CRED, LASER_OBJ, false, 5, 0.1, 1);
            laser->addAnchorKeyframe(new Coordinate(top->getX(), top->getY()));
            laser->addAnchorKeyframe(new Coordinate(toX, top->getY() - 1000));
            laser->addScaleKeyframe(1);
            laser->addScaleKeyframe(10);
            laser->addRotationKeyframe(player->getRotation());
            // laser->addRotationKeyframe(720);
            laser->moveWithoutAnchor(0, -10);

            objects->push_back(laser);
            delete top;
        }
    }
    endwin();
}

void draw(FrameBuffer* framebuffer, std::vector<Drawable*>* objects, bool* run) {
    Polygon* enemy = (Polygon*) objects->at(1);
    bool hit = false;

    while (*run) {
        framebuffer->clearScreen();

        for (int i = 0; i < objects->size(); i++) {
            objects->at(i)->draw(framebuffer);
            if (objects->at(i)->getId() != ENEMY_OBJ || !hit)
                objects->at(i)->animate();
            if (objects->at(i)->getId() == LASER_OBJ) {
                Animated* laser = (Animated*) objects->at(i);
                int dx = abs(laser->getAnchor()->getX() - enemy->getAnchor()->getX());
                int dy = abs(laser->getAnchor()->getY() - enemy->getAnchor()->getY());
                if (dx * dx + dy * dy <= 10000) {
                    hit = true;
                    Animated* explosion = new Animated("images/explosion.point", CYELLOW, EXPLOSION_OBJ, false, 0, 0.3, 0.1);
                    explosion->addAnchorKeyframe(new Coordinate(enemy->getAnchor()->getX(), enemy->getAnchor()->getY()));
                    explosion->addScaleKeyframe(1);
                    explosion->addScaleKeyframe(30);
                    explosion->addRotationKeyframe(0);
                    explosion->addRotationKeyframe(30);

                    objects->push_back(explosion);
                }
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

    Polygon* player = new Polygon("images/pesawat.point", CBLUE, PLAYER_OBJ);
    player->moveTo(framebuffer->getXRes() / 2, framebuffer->getYRes() - 80);
    player->scale(4);
    objects->push_back(player);

    Animated* enemy = new Animated("images/ufo.point", CMAGENTA, ENEMY_OBJ, true, 5, 0, 0);
    for (int i = 0; i < 10; i++) {
        int x = rand() % (framebuffer->getXRes() * 8 / 10) + (framebuffer->getXRes() / 10);
        int y = rand() % (framebuffer->getXRes() * 3 / 10) + (framebuffer->getXRes() / 10);
        enemy->addAnchorKeyframe(new Coordinate(x, y));
    }
    enemy->scale(4);
    objects->push_back(enemy);

    std::thread* t0 = new std::thread(readInput, framebuffer, objects, &run);
    std::thread* t1 = new std::thread(draw, framebuffer, objects, &run);

    t0->join();
    t1->join();

    delete player;
    delete enemy;
    delete t0;
    delete t1;
    delete framebuffer;

	return 0;
}