#include <curses.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "drawable.hpp"
#include "framebuffer.hpp"
#include "line.hpp"
#include "polygon.hpp"

#define COMMAND_QUIT 'q'
#define COMMAND_ROTATE_RIGHT 'm'
#define COMMAND_ROTATE_LEFT 'n'

void readInput(FrameBuffer* framebuffer, std::vector<Drawable*>* objects, bool* run) {
    initscr();
    timeout(-1);
    while (*run) {
        char c = getch();
        if (c == COMMAND_QUIT) {
            *run = false;
        } else if (c == COMMAND_ROTATE_RIGHT) {
            Polygon* pesawat = (Polygon*) objects->at(0);
            pesawat->rotate(0.2);
        } else if (c == COMMAND_ROTATE_LEFT) {
            Polygon* pesawat = (Polygon*) objects->at(0);
            pesawat->rotate(-0.2);
        }
    }
    endwin();
}

void draw(FrameBuffer* framebuffer, std::vector<Drawable*>* objects, bool* run) {
    while (*run) {
        framebuffer->clearScreen();
        for (int i = 0; i < objects->size(); i++) {
            objects->at(i)->draw(framebuffer);
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

    Polygon* pesawat = new Polygon("images/pesawat.point", CBLUE);
    pesawat->moveTo(framebuffer->getXRes() / 2, framebuffer->getYRes() - 40);
    pesawat->scale(3);
    objects->push_back(pesawat);

    std::thread* t0 = new std::thread(readInput, framebuffer, objects, &run);
    std::thread* t1 = new std::thread(draw, framebuffer, objects, &run);

    t0->join();
    t1->join();

	return 0;
}