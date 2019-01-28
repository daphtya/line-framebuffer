#include <vector>

#include "framebuffer.hpp"
#include "line.hpp"

using namespace std;

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

    int xres = framebuffer->getXRes();
    int yres = framebuffer->getYRes();

    std::vector<Line*> lines;

    lines.push_back(new Line(xres / 2, yres / 2, 4 * xres / 4, 2 * yres / 4, CBLUE, CRED));
    lines.push_back(new Line(xres / 2, yres / 2, 4 * xres / 4, 1 * yres / 4, CRED, CMAGENTA));
    lines.push_back(new Line(xres / 2, yres / 2, 4 * xres / 4, 0 * yres / 4, CGREEN, CYELLOW));
    lines.push_back(new Line(xres / 2, yres / 2, 3 * xres / 4, 0 * yres / 4, CBLUE, CGREEN));
    lines.push_back(new Line(xres / 2, yres / 2, 2 * xres / 4, 0 * yres / 4, CRED, CCYAN));
    lines.push_back(new Line(xres / 2, yres / 2, 1 * xres / 4, 0 * yres / 4, CGREEN, CWHITE));
    lines.push_back(new Line(xres / 2, yres / 2, 0 * xres / 4, 0 * yres / 4, CBLUE, CBLACK));
    lines.push_back(new Line(xres / 2, yres / 2, 0 * xres / 4, 1 * yres / 4, CRED, CGREEN));
    lines.push_back(new Line(xres / 2, yres / 2, 0 * xres / 4, 2 * yres / 4, CGREEN, CMAGENTA));
    lines.push_back(new Line(xres / 2, yres / 2, 0 * xres / 4, 3 * yres / 4, CBLUE, CYELLOW));
    lines.push_back(new Line(xres / 2, yres / 2, 0 * xres / 4, 4 * yres / 4, CRED, CBLACK));
    lines.push_back(new Line(xres / 2, yres / 2, 1 * xres / 4, 4 * yres / 4, CGREEN, CWHITE));
    lines.push_back(new Line(xres / 2, yres / 2, 2 * xres / 4, 4 * yres / 4, CBLUE, CCYAN));
    lines.push_back(new Line(xres / 2, yres / 2, 3 * xres / 4, 4 * yres / 4, CRED, CGREEN));
    lines.push_back(new Line(xres / 2, yres / 2, 4 * xres / 4, 4 * yres / 4, CGREEN, CWHITE));
    lines.push_back(new Line(xres / 2, yres / 2, 4 * xres / 4, 3 * yres / 4, CBLUE, CRED));

    for (Line* line : lines) {
        line->draw(framebuffer);
    }

    framebuffer->draw();
    getchar();

	return 0;
}