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
#include "physical.hpp"
#include "composite.hpp"

#define PI acos(-1)

#define COMMAND_QUIT ' '
#define COMMAND_ROTATE_LEFT 'q'
#define COMMAND_ROTATE_RIGHT 'e'
#define COMMAND_MOVE_LEFT 'a'
#define COMMAND_MOVE_RIGHT 'd'
#define COMMAND_SHOOT 'w'

void readInput(FrameBuffer *framebuffer, std::vector<Drawable *> *objects, bool *run)
{
    initscr();
    timeout(-1);
    while (*run)
    {
        char c = getch();
        if (c == COMMAND_QUIT)
        {
            *run = false;
        }
        else if (c == COMMAND_ROTATE_LEFT)
        {
            Polygon *player = (Polygon *)objects->at(0);
            if (player->getRotation() > -1.2)
            {
                player->rotate(-0.2);
            }
        }
        else if (c == COMMAND_ROTATE_RIGHT)
        {
            Polygon *player = (Polygon *)objects->at(0);
            if (player->getRotation() < 1.2)
            {
                player->rotate(0.2);
            }
        }
        else if (c == COMMAND_MOVE_LEFT)
        {
            Polygon *player = (Polygon *)objects->at(0);
            player->move(-3, 0);
        }
        else if (c == COMMAND_MOVE_RIGHT)
        {
            Polygon *player = (Polygon *)objects->at(0);
            player->move(3, 0);
        }
        else if (c == COMMAND_SHOOT)
        {
            Polygon *player = (Polygon *)objects->at(0);
            Coordinate *anchor = player->getAnchor();
            int toX = anchor->getX() + 1000 * tan(player->getRotation());
            int fromX = anchor->getX() - 40 * cos(PI / 2 + player->getRotation());
            int fromY = anchor->getY() - 40 * sin(PI / 2 + player->getRotation());

            Animated *laser = new Animated("images/laser.point", CRED, LASER_OBJ, false, 5, 0.1, 0.5, 0);
            laser->addAnchorKeyframe(new Coordinate(fromX, fromY));
            laser->addAnchorKeyframe(new Coordinate(toX, anchor->getY() - 1000));
            laser->addScaleKeyframe(1);
            laser->addScaleKeyframe(10);
            laser->addRotationKeyframe(player->getRotation());
            // laser->addRotationKeyframe(720);

            objects->push_back(laser);
        }
    }
    endwin();
}


Composite* createPlane(FrameBuffer* framebuffer){
    Animated* comp1 = new Animated("images/comp-plane1.point", CYELLOW, ENEMY_OBJ, true, 5, 0, 0, 5); //the nose of the plane
    Animated* comp2 = new Animated("images/comp-plane2.point", CMAGENTA, ENEMY_OBJ, true, 5, 0, 0, 4); //the body of the plane
    Animated* comp3 = new Animated("images/comp-plane3.point", CRED, ENEMY_OBJ, true, 5, 0, 0, 6); //the wing of the plane
    Animated* comp4 = new Animated("images/comp-plane3.point", CRED, ENEMY_OBJ, true, 5, 0, 0, 3); //the wing of the plane
    // comp1->setFillCoor(5,7);
    // comp2->setFillCoor(9, 3);
    // comp3->setFillCoor(11, 6);
    for (int i = 0; i < 10; i++)
    {
        int x = rand() % (framebuffer->getXRes() * 8 / 10) + (framebuffer->getXRes() / 10);
        int y = rand() % (framebuffer->getXRes() * 3 / 10) + (framebuffer->getXRes() / 10);
        comp1->addAnchorKeyframe(new Coordinate(x-3*4, y+4*4));
        comp2->addAnchorKeyframe(new Coordinate(x, y));
        comp3->addAnchorKeyframe(new Coordinate(x+3*4, y+2*4));
        comp4->addAnchorKeyframe(new Coordinate(x-3*4, y-2*4));
    }
    comp1->scale(4);
    comp2->scale(4);
    comp3->scale(4);
    comp4->scale(4);
    
    Composite *result = new Composite(4, ENEMY_OBJ);
    result->addAnimated(comp1);
    result->addAnimated(comp2);
    result->addAnimated(comp3);
    result->addAnimated(comp4);
    return result;
}

Animated* createBomb(FrameBuffer* framebuffer, Coordinate* coor) {
    Animated* bomb = new Animated("images/bomb.point", CCYAN, BOMB_OBJ, false, 2, 0, 0, 1);
    bomb->scale(4);
    bomb->addAnchorKeyframe(new Coordinate(coor->getX(), coor->getY()));
    bomb->addAnchorKeyframe(new Coordinate(coor->getX(), framebuffer->getYRes()+1));
    return bomb;
}

void draw(FrameBuffer *framebuffer, std::vector<Drawable *> *objects, bool *run)
{
    Composite *enemy = (Composite *)objects->at(1);
    std::vector<Animated*>* bombs = new std::vector<Animated*>();

    clock_t begin = clock();
    clock_t enemyrespawn = 0;
    bool hit = false;
    int life = 8;

    while (*run)
    {
        framebuffer->clearScreen();

        if (hit && (double)(clock()-enemyrespawn)/CLOCKS_PER_SEC > 3) {

            enemy = createPlane(framebuffer);
            (*objects)[1] = enemy;
            hit = false;
        }
        //printf("%lf \n", (double)(clock()-begin)/CLOCKS_PER_SEC);
        if (!hit && (double)(clock()-begin)/CLOCKS_PER_SEC > 2) {
            begin = clock();
            printf("dropping bombs\n");
            bombs->push_back(createBomb(framebuffer, enemy->getAnchor()));
        }

        for (int i = 0; i < bombs->size(); i++){
            if (!bombs->at(i)->isHidden()) {
                bombs->at(i)->draw(framebuffer);
                bombs->at(i)->animate();
                if (bombs->at(i)->getAnchor()->getY() >= framebuffer->getYRes()) {
                    life--;
                    Animated* smallExplosion = new Animated("images/explosion.point", CYELLOW, EXPLOSION_OBJ, false, 0, 0.5, 0.05, 2);
                    smallExplosion->addAnchorKeyframe(new Coordinate(bombs->at(i)->getAnchor()->getX(), bombs->at(i)->getAnchor()->getY()));
                    smallExplosion->addScaleKeyframe(2);
                    smallExplosion->addScaleKeyframe(5);
                    smallExplosion->addScaleKeyframe(2);
                    smallExplosion->addScaleKeyframe(5);
                    objects->push_back(smallExplosion);
                }
                if (life == 0) {
                    *run = false;
                }
            }
        }



        for (int i = 0; i < objects->size(); i++)
        {
            objects->at(i)->draw(framebuffer);
            if (objects->at(i)->getId() != ENEMY_OBJ || !hit)
                objects->at(i)->animate();
            if (objects->at(i)->getId() == LASER_OBJ && !hit)
            {
                Animated *laser = (Animated *)objects->at(i);
                int dx = abs(laser->getAnchor()->getX() - enemy->getAnchor()->getX());
                int dy = abs(laser->getAnchor()->getY() - enemy->getAnchor()->getY());
                if (dx * dx + dy * dy <= 2500)
                {
                    hit = true;
                    enemyrespawn = clock();
                    Animated *explosion = new Animated("images/explosion.point", CYELLOW, EXPLOSION_OBJ, false, 0, 0.5, 0.05, 2);
                    explosion->addAnchorKeyframe(new Coordinate(enemy->getAnchor()->getX(), enemy->getAnchor()->getY()));
                    for (int j = 0; j < 8; j++)
                    {
                        explosion->addScaleKeyframe(2);
                        explosion->addScaleKeyframe(10);
                    }
                    explosion->addRotationKeyframe(0);
                    explosion->addRotationKeyframe(13);

                    objects->push_back(explosion);
                    enemy->hide();

                    Physical *wreck1 = new Physical("images/comp-plane3.point", CRED, PHYSICAL_OBJ, -10, -10, 10);
                    wreck1->scale(4);
                    wreck1->addAnchorKeyframe(new Coordinate(enemy->getAnchor()->getX(), enemy->getAnchor()->getY()));
                    wreck1->addAnchorKeyframe(new Coordinate(enemy->getAnchor()->getX() - 5, enemy->getAnchor()->getY() - 10));

                    Physical *wreck2 = new Physical("images/comp-plane2.point", CMAGENTA, PHYSICAL_OBJ, 10, -10, 10);
                    wreck2->scale(4);
                    wreck2->addAnchorKeyframe(new Coordinate(enemy->getAnchor()->getX(), enemy->getAnchor()->getY()));
                    wreck2->addAnchorKeyframe(new Coordinate(enemy->getAnchor()->getX() + 5, enemy->getAnchor()->getY() - 10));

                    objects->push_back(wreck1);
                    objects->push_back(wreck2);
                }

                for (int j = 0; j < bombs->size(); j++) {
                    if (!bombs->at(j)->isHidden()){
                        int bomb_dx = abs(laser->getAnchor()->getX() - bombs->at(j)->getAnchor()->getX());
                        int bomb_dy = abs(laser->getAnchor()->getY() - bombs->at(j)->getAnchor()->getY());
                        if (bomb_dx * bomb_dx + bomb_dy * bomb_dy <= 1600) {
                            Animated* smallExplosion = new Animated("images/explosion.point", CYELLOW, EXPLOSION_OBJ, false, 0, 0.5, 0.05, 2);
                            smallExplosion->addAnchorKeyframe(new Coordinate(bombs->at(j)->getAnchor()->getX(), bombs->at(j)->getAnchor()->getY()));
                            smallExplosion->addScaleKeyframe(2);
                            smallExplosion->addScaleKeyframe(5);
                            smallExplosion->addScaleKeyframe(2);
                            smallExplosion->addScaleKeyframe(5);
                            objects->push_back(smallExplosion);
                            bombs->at(j)->hide();
                        }
                    }
                }

            }
        }
        framebuffer->draw();
        usleep(10000);
    }
}



int main(int argc, char **args)
{
    FrameBuffer *framebuffer;
    try
    {
        framebuffer = new FrameBuffer();
    }
    catch (Exception e)
    {
        e.print();
        return 1;
    }

    bool run = true;
    std::vector<Drawable *> *objects = new std::vector<Drawable *>;

    Polygon *player = new Polygon("images/pesawat.point", CBLUE, PLAYER_OBJ);
    player->moveTo(framebuffer->getXRes() / 2, framebuffer->getYRes() - 80);
    player->scale(4);
    objects->push_back(player);

    // Animated *enemy = new Animated("images/ufopolos.point", CMAGENTA, ENEMY_OBJ, true, 5, 0, 0);
    // for (int i = 0; i < 10; i++)
    // {
    //     int x = rand() % (framebuffer->getXRes() * 8 / 10) + (framebuffer->getXRes() / 10);
    //     int y = rand() % (framebuffer->getXRes() * 3 / 10) + (framebuffer->getXRes() / 10);
    //     enemy->addAnchorKeyframe(new Coordinate(x, y));
    // }
    // enemy->scale(4);
    Composite* enemy = createPlane(framebuffer);
    objects->push_back(enemy);

    std::thread *t0 = new std::thread(readInput, framebuffer, objects, &run);
    std::thread *t1 = new std::thread(draw, framebuffer, objects, &run);

    t0->join();
    t1->join();

    delete player;
    delete enemy;
    delete t0;
    delete t1;
    delete framebuffer;

    return 0;
}