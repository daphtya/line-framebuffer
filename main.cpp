
#include "utilities.h"
#include "DrawnObject.cpp"
#include "Pesawat.cpp"

using namespace std;

int main(int argc, char **args) {
	int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char* fbp = 0;
    int i, j;
    int x = 0, y = 0;

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    // Initialize screen information
    initScreenInfo(vinfo, finfo);

    // Draw diagonal line
    clear_screen(fbp, screensize);
    Line line(0, 0, vinfo.xres/2, vinfo.yres);
    Line line2(0,0, vinfo.xres, vinfo.yres/2);
    Line line3(0, vinfo.yres, vinfo.xres/2, 0);
    Line line4(0, vinfo.yres, vinfo.xres, vinfo.yres/2);
    Line line5(0, vinfo.yres/2, vinfo.xres, vinfo.yres/2);
    Pesawat p1 (vinfo.xres/2, vinfo.yres/2, true);

    line.draw(fbp, BLUE | RED); //purple
    line2.draw(fbp, RED | GREEN); //yellow
    line3.draw(fbp, BLUE);
    line4.draw(fbp, BLUE | GREEN);
    line5.draw(fbp, GREEN);

    //draw loop

    while (true) {
        // update position of all objects
        p1.update();
        // draw objects
        p1.draw(fbp);
        //delay before next frame
        usleep(1000);
        clear_screen(fbp, screensize);
    }


	return 0;
}