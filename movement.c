#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <math.h>

// "running average" for the last 20 px
int getQuadrant(int deltax, int deltay) {
    int quadrant;
    deltax = deltax == 0 ? 1 : deltax;
    deltay = deltay == 0 ? 1 : deltay;

    double slope = (double)deltax / (double)deltay;
    if (slope > 0 && deltax >= 0) quadrant = 1;
    if (slope < 0 && deltax <= 0) quadrant = 2; 
    if (slope > 0 && deltax <= 0) quadrant = 3; 
    if (slope < 0 && deltax >= 0) quadrant = 4; 

    return quadrant;
}

int main() {
    Display *display;
    XEvent xevent;
    Window window;
    int prevx, prevy, oldquadrant, quadrant = 0;
    int out = -1;

    if( (display = XOpenDisplay(NULL)) == NULL )
        return -1;

    window = DefaultRootWindow(display);
    XAllowEvents(display, AsyncBoth, CurrentTime);

    XGrabPointer(display, window, 1, PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);

    for (;;) {
        out += XNextEvent(display, &xevent) + 1;

        if (out % 20 == 0) {
            int x = xevent.xmotion.x_root;
            int y = xevent.xmotion.y_root;
            
            oldquadrant = quadrant;
            quadrant = getQuadrant(x - prevx, prevy - y);
            prevx = x;
            prevy = y;

            if (oldquadrant != quadrant) {
                printf("switch\n");
            }
        }

        // usleep(10000);
        // printf("x: %d, y: %d\n", xevent.xmotion.x_root, xevent.xmotion.y_root);
        // printf("dir: %d\n", dir);
    }

    return 0;
}