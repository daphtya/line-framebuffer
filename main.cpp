#include <curses.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "drawable.hpp"
#include "framebuffer.hpp"
#include "line.hpp"

bool run;

void readInput(FrameBuffer* framebuffer, vector<Drawable*>* objects) {
    initscr();
    timeout(-1);
    while (run) {
        char c = getch();
        if (c == 'q') {
            run = false;
        } else if (c == 'a') {
            Drawable* newObject = (Drawable*) new Line(0, 0, 1366, 768);
            newObject->draw(framebuffer);
            objects->push_back(newObject);
        } else if (c == 's') {
            Drawable* newObject = (Drawable*) new Line(1366, 0, 0, 768);
            newObject->draw(framebuffer);
            objects->push_back(newObject);
        }
    }
    endwin();
}

void draw(FrameBuffer* framebuffer) {
    while (run) {
        framebuffer->draw();
        usleep(10000);
    }
}

int main(int argc, char **args) {
    char* fbp = 0;
    int i, j;
    int x = 0, y = 0;

    FrameBuffer* framebuffer;

    try {
        framebuffer = new FrameBuffer();
    } catch (Exception e) {
        e.print();
        return 1;
    }

    framebuffer->clearScreen();

    run = true;
    std::vector<Drawable*>* objects = new std::vector<Drawable*>;

    std::thread* t0 = new std::thread(readInput, framebuffer, objects);
    std::thread* t1 = new std::thread(draw, framebuffer);

    t0->join();
    t1->join();

	return 0;
}