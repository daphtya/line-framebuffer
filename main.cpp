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
#define COMMAND_MOVE_LEFT 'a'
#define COMMAND_MOVE_RIGHT 'd'
#define COMMAND_MOVE_UP 'w'
#define COMMAND_MOVE_DOWN 's'
#define COMMAND_ZOOM_IN '='
#define COMMAND_ZOOM_OUT '-'

void readInput(FrameBuffer *framebuffer, std::vector<Drawable *> *objects, bool *run)
{
    initscr();
    timeout(-1);
    Composite *map = (Composite *)objects->at(0);
    
    while (*run)
    {
        char c = getch();
        if (c == COMMAND_QUIT)
        {
            *run = false;
        }
        else if (c == COMMAND_MOVE_LEFT)
        {
            map->moveWithoutAnchor(-3, 0);
        }
        else if (c == COMMAND_MOVE_RIGHT)
        {
            map->moveWithoutAnchor(3, 0);
        }
        else if (c == COMMAND_MOVE_UP)
        {
            map->moveWithoutAnchor(0, -3);
        }
        else if (c == COMMAND_MOVE_DOWN)
        {
            map->moveWithoutAnchor(0, 3);
        }
        else if (c == COMMAND_ZOOM_IN)
        {
            map->scale(2);
        }
        else if (c == COMMAND_ZOOM_OUT)
        {
            map->scale(0.5);
        }
    }
    endwin();
}


Composite* createMap(FrameBuffer* framebuffer){
    int buildingNum = 53;
    std::string listFile[buildingNum] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
        "AA","AB","AC","AD","AE","AF","AG","AH","AI","AJ","AK","AL","AM","AN","AO","AP","AQ","AR","AS","AT","AU","AV","AW","AX","AY","AZ","BA"};
    Animated* building[buildingNum];
    Composite *result = new Composite(buildingNum, ENEMY_OBJ);
    std::string temp;
    
    for (int i = 0; i < buildingNum; i++) {
        temp = "images/bangunanITB/"+listFile[i]+".point";
        building[i] = new Animated(temp, CWHITE, ENEMY_OBJ, false, 0, 0, 0, i);
        building[i]->setAnchor(0, 0);
        building[i]->scale(8);
        result->addAnimated(building[i]);
    }
    return result;
}

void draw(FrameBuffer *framebuffer, std::vector<Drawable *> *objects, bool *run)
{
    while (*run)
    {
        framebuffer->clearScreen();
        for (int i = 0; i < objects->size(); i++)
        {
            objects->at(i)->draw(framebuffer);
            if (objects->at(i)->getId() != ENEMY_OBJ)
                objects->at(i)->animate();
        }
        framebuffer->draw();
        usleep(10000);
    }
}



int main(int argc, char **args)
{
    flogclear();
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
    
    Composite* mapITB = createMap(framebuffer);
    objects->push_back(mapITB);

    std::thread *t0 = new std::thread(readInput, framebuffer, objects, &run);
    std::thread *t1 = new std::thread(draw, framebuffer, objects, &run);

    t0->join();
    t1->join();

    delete mapITB;
    delete t0;
    delete t1;
    delete framebuffer;

    return 0;
}