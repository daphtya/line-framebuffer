#include "framebuffer.cpp"
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

    Line* line1 = new Line(0, 0, xres / 2, yres);
    Line* line2 = new Line(0, 0, xres, yres / 2);
    Line* line3 = new Line(0, yres, xres / 2, 0);
    Line* line4 = new Line(0, yres, xres, yres / 2);
    Line* line5 = new Line(0, yres / 2, xres, yres / 2);

    line1->draw(framebuffer);
    line2->draw(framebuffer);
    line3->draw(framebuffer);
    // line4->draw(framebuffer);
    // line5->draw(framebuffer);

    framebuffer->draw();
    getchar();

	return 0;
}