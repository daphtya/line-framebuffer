#include "utilities.h"

struct pixel** allocate_logo(int height, int width) {
    int i;

    struct pixel** logo = malloc(height * sizeof(struct pixel*));
    for (i = 0; i < height; i++) {
        logo[i] = malloc(width * sizeof(struct pixel));
    }

    return logo;
}

void read_logo(struct pixel** logo, char* filename, int height, int width) {
    FILE* logo_file;
    int b, g, r;
    int i, j;

    logo_file = fopen (filename, "r");
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fscanf(logo_file, "%d %d %d", &b, &g, &r);
            logo[i][j].blue = b;
            logo[i][j].green = g;
            logo[i][j].red = r;
        }
    }
    fclose(logo_file);
}

void* animate(void* arg) {
    // x1, y1 : initial position
    // x2, y2 : destination
    // w, h : width, height
    args* data = (args *) arg;
    int i;
    int xl, xr, yt, yb; // left x, right x, top y, bottom y
    int x = 0, y = 0;
    int duration;
    long int location = 0;
    float dx, dy; // delta x, delta y
    float fx, fy; // floating number coordinate
    char* fbp = data->fbp;
    struct pixel** logo = data->logo;
    struct fb_var_screeninfo vinfo = data->vinfo;
    struct fb_fix_screeninfo finfo = data->finfo;

    // Initial Position
    xl = data->x1; // Place the logo on the center
    yt = data->y1; // Initiate the y coordinate at the very bottom of the screen
    int x2 = data->x2;
    int y2 = data->y2;
    
    // Actual position and deltas
    fx = xl;
    fy = yt;
    
    // xl = x1, yt = y1
    if (abs(x2-xl) > abs(y2-yt))
        duration = abs(x2-xl);
    else
        duration = abs(y2-yt);
        
    dx = (float) (x2 - xl)/duration;
    dy = (float) (y2 - yt)/duration;
    
    // Animation
    for (i = 0; i < duration; i++) {

        xr = xl + data->w;
        yb = yt + data->h;

        // Figure out where in memory to put the pixel
        for (y = yt; y < yb; y++)
            // Restriction to "paint" only pixels that actually on the screen
            if (y >= 0 && y < vinfo.yres - 6)
                for (x = xl; x < xr; x++) {
                    if (x >= 0 && x < vinfo.xres - 6) {
                        location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                                   (y+vinfo.yoffset) * finfo.line_length;

                        // Assumption = Color depth 32bit
                        *(fbp + location) = logo[y - yt][x - xl].blue;        // Some blue
                        *(fbp + location + 1) = logo[y - yt][x - xl].green;     // A little green
                        *(fbp + location + 2) = logo[y - yt][x - xl].red;    // A lot of red
                        *(fbp + location + 3) = 0;      // No transparency
                    }
                }
        fx += dx; fy += dy;
        xl = fx; yt = fy;
        usleep(data->delay); // Delays each animation loop for a specified microsecond
    }
}

void clear_screen(char* fbp, long int screensize){
    memset(fbp, 0, screensize);
}

args convert_to_args(int x1, int y1, int x2, int y2, struct pixel** logo, int w, int h, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, char* fbp, int delay) {
    args temp;

    temp.x1 = x1;
    temp.y1 = y1;
    temp.x2 = x2;
    temp.y2 = y2;
    temp.logo = logo;
    temp.w = w;
    temp.h = h;
    temp.vinfo = vinfo;
    temp.finfo = finfo;
    temp.fbp = fbp;
    temp.delay = delay;

    return temp;
}