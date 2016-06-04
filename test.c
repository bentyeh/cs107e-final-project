#include "gl.h"
#include "printf.h"
#include "assert.h"
#include "timer.h"
#include "drumimage.h"
#include "minimath.h"

/* Note that to use the console, one should only have to
 * call console_init. To use the graphics library, one
 * should only have to call gl_init. If your main() requires
 * more than this, then your code will not pass tests and
 * will likely have many points deducted. Our GL tests
 * will call gl_init then invoke operations; our console
 * tests will call console_init then invoke operations.
 * To guarantee that tests will pass, make sure to also
 * run tests for each component separately.
 */

#define _WIDTH 800
#define _HEIGHT 600

#define NUMOFDRUMS 4
#define BEAT_LENGTH 200000

void test_gl() {
    // Initialize graphics library with specified buffer (single or double)
    drumimage_init(_WIDTH, _HEIGHT, NUMOFDRUMS);
    while(1) {
        for(int i = 0; i < NUMOFDRUMS; i++) {
            beat_drum(i, BEAT_LENGTH    );
        }
    }
}

void test_math() {
    printf("pow(1,5) = %d\n", (int)pow(1,5));
    printf("pow(2,5) = %d\n", (int)pow(2,5));
    printf("factorial(5) = %d\n", factorial(5));
    printf("factorial(0) = %d\n", factorial(0));
    printf("factorial(-1) = %d\n", factorial(-1));
    for(int i = 0; i < 4*MATH_PI; i+= (4*MATH_PI/12)) {
        printf("cos(-%d) = %d\n", (int)(1000*i), (int)(1000*cos(-i)));
    }
}

void main() {
    delay(3);

    // printf("Beginning graphics testing...\n");
    // test_gl();
    // printf("Graphics testing finished.\n");

    printf("Beginning math testing...\n");
    test_math();
    printf("Math testing finished.\n");
}
