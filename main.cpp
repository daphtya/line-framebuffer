#include "framebuffer.cpp"
#include <vector>
#include "line.cpp"

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

    lines.push_back(new Line(xres / 2, yres / 2, 4 * xres / 4, 2 * yres / 4, CBLUE, CBLUE));
    lines.push_back(new Line(xres / 2, yres / 2, 4 * xres / 4, 1 * yres / 4, CRED, CRED));
    lines.push_back(new Line(xres / 2, yres / 2, 4 * xres / 4, 0 * yres / 4, CGREEN, CGREEN));
    lines.push_back(new Line(xres / 2, yres / 2, 3 * xres / 4, 0 * yres / 4, CBLUE, CBLUE));
    lines.push_back(new Line(xres / 2, yres / 2, 2 * xres / 4, 0 * yres / 4, CRED, CRED));
    lines.push_back(new Line(xres / 2, yres / 2, 1 * xres / 4, 0 * yres / 4, CGREEN, CGREEN));
    lines.push_back(new Line(xres / 2, yres / 2, 0 * xres / 4, 0 * yres / 4, CBLUE, CBLUE));
    lines.push_back(new Line(xres / 2, yres / 2, 0 * xres / 4, 1 * yres / 4, CRED, CRED));
    lines.push_back(new Line(xres / 2, yres / 2, 0 * xres / 4, 2 * yres / 4, CGREEN, CGREEN));
    lines.push_back(new Line(xres / 2, yres / 2, 0 * xres / 4, 3 * yres / 4, CBLUE, CBLUE));
    lines.push_back(new Line(xres / 2, yres / 2, 0 * xres / 4, 4 * yres / 4, CRED, CRED));
    lines.push_back(new Line(xres / 2, yres / 2, 1 * xres / 4, 4 * yres / 4, CGREEN, CGREEN));
    lines.push_back(new Line(xres / 2, yres / 2, 2 * xres / 4, 4 * yres / 4, CBLUE, CBLUE));
    lines.push_back(new Line(xres / 2, yres / 2, 3 * xres / 4, 4 * yres / 4, CRED, CRED));
    lines.push_back(new Line(xres / 2, yres / 2, 4 * xres / 4, 4 * yres / 4, CGREEN, CGREEN));
    lines.push_back(new Line(xres / 2, yres / 2, 4 * xres / 4, 3 * yres / 4, CBLUE, CBLUE));

    for (Line* line : lines) {
        line->draw(framebuffer);
    }

    framebuffer->draw();
    getchar();

	return 0;
}