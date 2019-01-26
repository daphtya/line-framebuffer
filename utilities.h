#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

struct pixel {
    int red;
    int green;
    int blue;
};

typedef struct _args {
    int x1; int y1;
    int x2; int y2;
    struct pixel** logo;
    int w; 
    int h;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    char* fbp;
    int delay;
} args;

#define LOGO_HEIGHT 204
#define LOGO_WIDTH 750
#define NAME_HEIGHT 76
#define NAME_WIDTH 761
#define NUM_IMAGE 7

struct pixel** allocate_logo(int height, int width);
void read_logo(struct pixel** logo, char* filename, int height, int width);
void* animate(void* arg);
void clear_screen(char *, long int);
args convert_to_args(int x1, int y1, int x2, int y2, struct pixel** logo, int w, int h, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char* fbp, int delay);