#include "utilities.h"
#include "DrawnObject.cpp"
#include "Pesawat.cpp"
#include "Missile.cpp"

using namespace std;

int main(int argc, char **args) {
	int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char* fbp = 0;
    int i, j;

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

    /**
     * Defining Virtual Framebuffer
     * 
     * Virtual framebuffer is created to avoid object flickering in the screen.
     * 
     * Instead of drawing any object directly to the framebuffer device,
     * any object will be drawn in the virtual framebuffer.
     * The frame loop will copy content of virtual framebuffer to the framebuffer device later.
     */  
    char *bbp = new char[screensize];  

    /* Screen Initialization */
    initScreenInfo(bbp, vinfo, finfo);
    clear_screen(fbp, screensize);

    /* Object Initialization */    
    // Plane
    Pesawat p1 (vinfo.xres/2, vinfo.yres/2, true);

    // DrawnObject
    int x = vinfo.xres/2;
    int y = vinfo.yres/2;
    int tail_disp = 30;
    DrawnObject obj(3);
    Line pline1 (x,y,x+tail_disp, y+10);
    Line pline2 (x,y,x+tail_disp, y-10);
    Line pline3 (x+tail_disp, y+10, x+tail_disp, y-10);
    obj.addLine(pline1);
    obj.addLine(pline2);
    obj.addLine(pline3);

    // Missile
    Missile missile(0, 0, vinfo.xres, vinfo.yres, 500);

    int counter = 150;

     while (true) {
        // draw objects
        missile.update();
        p1.draw(RED);
        obj.draw(GREEN);
        if (counter == 300) {
            counter = 0;
            p1.swapDirection();
        }
        // update position of all objects
        //delay before next frame
        memcpy(fbp, bbp, screensize);
        usleep(1000);
        clear_screen(bbp, screensize);

        counter ++;
        p1.update();
        obj.moveObject(1,0);

        //clear_screen(fbp, screensize);
     }


	return 0;
}