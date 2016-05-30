#include "gl.h"
#include "minimath.h"
#include "font.h"
// gl.h already includes fb.h

#define RGBA_DEPTH 4

// Function prototypes
static void swap(int *x, int *y);
static int isValid(int x, int y);
// static color blend(color c, color bg, unsigned overlap);
static color blendrgb(char r_c, char g_c, char b_c, color bg, unsigned overlap);
static void drawBottomFlatTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color c);
static void drawTopFlatTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color c);
static void sortTriangleVertices(int *x1, int *y1, int *x2, int *y2, int *x3, int *y3);
void gl_draw_hline(int y, int x1, int x2, color c);
void gl_draw_vline(int x, int y1, int y2, color c);

/* BASIC GRAPHICS LIBRARY */

void gl_init(unsigned width, unsigned height, unsigned mode) {
    fb_init(width, height, RGBA_DEPTH, mode);
}

unsigned gl_get_width(void) {
    return fb_get_width();
}

unsigned gl_get_height(void) {
    return fb_get_height();
}

void gl_swap_buffer() {
    fb_swap_buffer();
}

color gl_color(unsigned char r, unsigned char g, unsigned char b) {
    unsigned char a = 0xFF;
    return (a << 24) + (b << 16) + (g << 8) + r;
}

// Code based on cleari.c from CS 107E, Spring 2016, Lecture 11
void gl_clear(color c) {
    unsigned *im = (unsigned *)fb_get_draw_buffer();

    int n = fb_get_pitch()/4 * fb_get_height();
    for (int i = 0; i < n; i++) {
        *im++ = c;
    }
}

// Code based on grid.c from CS 107E, Spring 2016, Lecture 11
void gl_draw_pixel(int x, int y, color c) {
    // Check that the pixel location specified is valid
    if(isValid(x,y)) {
        int width = fb_get_pitch()/4;
        unsigned (*im)[width] = (unsigned (*)[width])fb_get_draw_buffer();
        im[y][x] = c;
    }
}

color gl_read_pixel(int x, int y) {
    // Check that the pixel location specified is valid
    if(isValid(x,y)) {
        int width = fb_get_pitch()/4;
        unsigned (*im)[width] = (unsigned (*)[width])fb_get_draw_buffer();
        return (color)im[y][x];
    }

    return 0;
}

void gl_draw_rect(int x, int y, int w, int h, color c) {
    int i, j, width, height;

    width = fb_get_width();
    height = fb_get_height();
    for(i = 0; i < w && (x+i) < width; i++) {
        for(j = 0; j < h && (y+j) < height; j++) {
            gl_draw_pixel(x+i, y+j, c);
        }
    }
}

/* ADD TEXT */

void gl_draw_char(int x, int y, char ch, color c) {

    // Check that characters will fit in the frame
    if(font_get_width() > fb_get_width()) {
        return;
    }

    // Check for valid y coordinate
    if(y > (fb_get_height() - font_get_height())) {
        return;
    }

    // Draw each pixel of the character
    // Assumes font_get_depth() == 4
    int i, j;                                                   // declare variables
    int buf[font_get_height()][font_get_width()];               // 2-D array of pixels
    if(font_get_char(ch, (char *)(&buf), font_get_size())) {    // Check for valid character
        for(j = 0; j < font_get_height(); j++) {
            for(i = 0; i < font_get_width(); i++) {
                if(buf[j][i]) {
                    gl_draw_pixel(x+i, y+j, c);
                }
            }
        }
    }
}

void gl_draw_string(int x, int y, char* str, color c) {

    // Check that characters will fit in the frame
    if(font_get_width() > fb_get_width()) {
        return;
    }

    // Check for valid y coordinate
    if(y > (fb_get_height() - font_get_height())) {
        return;
    }

    // Draw each character
    int i = 0;
    while(str[i] != '\0' && (x+font_get_width() <= fb_get_width())) {
        gl_draw_char(x, y, str[i], c);
        x += font_get_width();
        i++;
    }
}

void gl_draw_vline(int x, int y1, int y2, color c) {
    int i;

    // Make sure that y2 > y1
    if(y1 > y2) {
        swap(&y1, &y2);
    }

    for(i = y1; i <= y2; i++) {
        gl_draw_pixel(x, i, c);
    }
}

void gl_draw_hline(int y, int x1, int x2, color c) {
    int i;

    // Make sure that y2 > y1
    if(x1 > x2) {
        swap(&x1, &x2);
    }

    for(i = x1; i <= x2; i++) {
        gl_draw_pixel(i, y, c);
    }
}

/* Draw anti-aliased lines using Xiaolin Wu's Line Algorithm
 * - Based on psuedocode from https://en.wikipedia.org/wiki/Xiaolin_Wu's_line_algorithm
 * - Simplification from the assumption (applied here) that input coordinates
 *   to gl_draw_line() are integers, not floating point values.
 * - Also draws colored lines, blending against the background pixel
 */
void gl_draw_line(int x1, int y1, int x2, int y2, color c) {
    // Trivial case: vertical line
    if(x1 == x2) {
        gl_draw_vline(x1, y1, y2, c);
        return;
    }

    // Trivial case: horizontal line
    if(y1 == y2) {
        gl_draw_hline(y1, x1, x2, c);
        return;
    }

    int steep, x;
    double gradient, y;
    char r_c, g_c, b_c;
    color c0, c1, bg0, bg1;

    // Draw endpoints
    gl_draw_pixel(x1, y1, c);
    gl_draw_pixel(x2, y2, c);

    // Ensure that x2 > x1
    if(x1 > x2) {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    // If slope is greater than 1
    steep = abs(y2 - y1) > abs(x2 - x1);
    if(steep) {
        swap(&x1, &y1);
        swap(&x2, &y2);
    }

    // Get slope
    gradient = (double) (y2 - y1) / (x2 - x1);

    // Separate color c into rgb components
    r_c = (char)(c & 0xFF);
    g_c = (char)((c >> 8) & 0xFF);
    b_c = (char)((c >> 16) & 0xFF);

    // main loop
    y = y1 + gradient;
    for(x = x1 + 1; x < x2; x++) {
        if(steep) {
            bg0 = gl_read_pixel((int)y, x);
            bg1 = gl_read_pixel((int)y + 1, x);
            c0 = blendrgb(r_c, g_c, b_c, bg0, (unsigned)(rfpart(y)*255));
            c1 = blendrgb(r_c, g_c, b_c, bg1, (unsigned)(fpart(y)*255));
            gl_draw_pixel((int)y,     x, c0);
            gl_draw_pixel((int)y + 1, x, c1);
        }
        else {
            bg0 = gl_read_pixel(x, (int)y);
            bg1 = gl_read_pixel(x, (int)y + 1);
            c0 = blendrgb(r_c, g_c, b_c, bg0, (unsigned)(rfpart(y)*255));
            c1 = blendrgb(r_c, g_c, b_c, bg1, (unsigned)(fpart(y)*255));
            gl_draw_pixel(x, (int)y,     c0);
            gl_draw_pixel(x, (int)y + 1, c1);
        }
        y += gradient;
    }
}

/* Draw a solid triangle given three vertices and a specified color.
 * The edges of the triangle are anti-aliased.
 * 
 * Algorithm based on psuedocode from http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
 */
void gl_draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, color c) {
    int x4, y4;

    // Sort vertices by their y-coordinate so that y1 <= y2 <= y3
    sortTriangleVertices(&x1, &y1, &x2, &y2, &x3, &y3);

    // Trivial case of bottom-flat triangle
    if (y2 == y3) {
        drawBottomFlatTriangle(x1, y1, x2, y2, x3, y3, c);
    }

    // Trivial case of top-flat triangle
    else if (y1 == y2) {
        drawTopFlatTriangle(x1, y1, x2, y2, x3, y3, c);
    }

    // General case - split the triangle into a top-flat and a bottom-flat triangle
    else {
        x4 = x1 + (y2 - y1) * (double) (x3 - x1) / (y3 - y1);
        y4 = y2;
        drawBottomFlatTriangle(x1, y1, x2, y2, x4, y4, c);
        drawTopFlatTriangle(x2, y2, x4, y4, x3, y3, c);
    }

    // Draw anti-aliased lines for edges of triangle
    gl_draw_line(x1, y1, x2, y2, c);
    gl_draw_line(x1, y1, x3, y3, c);
    gl_draw_line(x2, y2, x3, y3, c);
}

// Assumes that (x1, y1) is the top vertex
static void drawBottomFlatTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color c) {
    int y;
    double m1, m2, curx1, curx2;
    sortTriangleVertices(&x1, &y1, &x2, &y2, &x3, &y3);

    // m1, m2 are inverse slopes, dx/dy
    m1 = (double) (x2 - x1) / (y2 - y1);
    m2 = (double) (x3 - x1) / (y3 - y1);

    curx1 = x1;
    curx2 = x1;
    for (y = y1; y <= y2; y++) {
        gl_draw_line((int)curx1, y, (int)curx2, y, c);
        curx1 += m1;
        curx2 += m2;
    }
}

static void drawTopFlatTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color c) {
    int y;
    double m1, m2, curx1, curx2;
    sortTriangleVertices(&x1, &y1, &x2, &y2, &x3, &y3);

    // m1, m2 are inverse slopes, dx/dy
    m1 = (double) (x3 - x1) / (y3 - y1);
    m2 = (double) (x3 - x2) / (y3 - y2);

    // curx1 = x3;
    // curx2 = x3;
    // for (y = y3-1; y >= y1; y--) {
    //     gl_draw_line((int)curx1, y, (int)curx2, y, c);
    //     curx1 -= m1;
    //     curx2 -= m2;
    // }
    curx1 = x1;
    curx2 = x2;
    for(y = y1; y < y3; y++) {
        gl_draw_line((int)curx1, y, (int)curx2, y, c);
        curx1 += m1;
        curx2 += m2;
    }
}

// End up with y3 >= y2 >= y1. For vertices with the same y, sort by x
static void sortTriangleVertices(int *x1, int *y1, int *x2, int *y2, int *x3, int *y3) {
    if(*y1 > *y2) {
        swap(x2, x1);
        swap(y2, y1);
    }
    if(*y2 > *y3) {
        swap(x3, x2);
        swap(y3, y2);
    }
    if(*y1 > *y2) {
        swap(x2, x1);
        swap(y2, y1);
    }
    if(*y1 == *y2) {
        if(*x1 > *x2) {
            swap(x2, x1);
            swap(y2, y1);
        }
    }
    if(*y2 == *y3) {
        if(*x2 > *x3) {
            swap(x3, x2);
            swap(y3, y2);
        }
    }
}

static void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

static int isValid(int x, int y) {
    return (x >= 0 && x < fb_get_width() &&
            y >= 0 && y < fb_get_height());
}

/*static color blend(color c, color bg, unsigned overlap) {
    char r_c, g_c, b_c, r_bg, g_bg, b_bg, r, g, b;
    r_c = (char)(c & 0xFF);
    g_c = (char)((c >> 8) & 0xFF);
    b_c = (char)((c >> 16) & 0xFF);
    r_bg = (char)(bg & 0xFF);
    g_bg = (char)((bg >> 8) & 0xFF);
    b_bg = (char)((bg >> 16) & 0xFF);
    r = (r_c * overlap + r_bg * (255-overlap)) / 255;
    g = (g_c * overlap + g_bg * (255-overlap)) / 255;
    b = (b_c * overlap + b_bg * (255-overlap)) / 255;
    return gl_color(r,g,b);
}*/

static color blendrgb(char r_c, char g_c, char b_c, color bg, unsigned overlap) {
    char r_bg, g_bg, b_bg, r, g, b;
    r_bg = (char)(bg & 0xFF);
    g_bg = (char)((bg >> 8) & 0xFF);
    b_bg = (char)((bg >> 16) & 0xFF);
    r = (r_c * overlap + r_bg * (255-overlap)) / 255;
    g = (g_c * overlap + g_bg * (255-overlap)) / 255;
    b = (b_c * overlap + b_bg * (255-overlap)) / 255;
    return gl_color(r,g,b);
}