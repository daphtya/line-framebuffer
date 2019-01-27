#ifndef _UTILITIES_H
#define _UTILITIES_H

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

typedef struct fb_var_screeninfo fb_vinfo;
typedef struct fb_fix_screeninfo fb_finfo;
typedef unsigned int int32;

/**
 * Global variables which store screen information
 * Initialized by calling initScreenInfo procedure
 */ 
extern int32 X_RES;
extern int32 Y_RES;
extern int32 X_OFFSET;
extern int32 Y_OFFSET;
extern int32 BITS_PER_PIXEL;
extern int32 X_LENGTH;
extern int32 Y_LENGTH;

/*
 * Framebuffer device
 */
extern char* FBP;

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
    fb_vinfo vinfo;
    fb_finfo finfo;
    char* fbp;
    int delay;
} args;

#define LOGO_HEIGHT 204
#define LOGO_WIDTH 750
#define NAME_HEIGHT 76
#define NAME_WIDTH 761
#define NUM_IMAGE 7

/**
 * Integer representation of color
 */ 
#define RED 16711680
#define GREEN 65280
#define BLUE 255

struct pixel** allocate_logo(int height, int width);
void read_logo(struct pixel** logo, char* filename, int height, int width);
void* animate(void* arg);
void clear_screen(char *, long int);
args convert_to_args(int x1, int y1, int x2, int y2, struct pixel** logo, int w, int h, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char* fbp, int delay);

/**
 * Initialize information about screen
 * In order to use putpixel, this procedure must be called before any call of putpixel.
 * 
 * @input fbp   - the framebuffer device
 * @input vinfo - retrieved by calling FBIOGET_VSCREENINFO using ioctl
 * @input finfo - retrieved by calling FBIOGET_FSCREENINFO using ioctl
 */ 
void initScreenInfo(char* fbp, fb_vinfo vinfo, fb_finfo finfo);
/**
 * Put pixel to specified position (x, y) on the screen
 * 
 * @input x     - x position in the screen
 * @input y     - y position in the screen
 * @input color - 24 bits representation of color: red | green | blue. 
 *                Example for red: 11111111 00000000 00000000 or as integer 16711680
 */ 
void putpixel(int x, int y, int32 color);

#endif