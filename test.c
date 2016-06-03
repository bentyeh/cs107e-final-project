#include "gl.h"
#include "printf.h"
#include "assert.h"
#include "timer.h"
#include "drumimage.h"

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

void main() {
    delay(3);

    printf("Beginning graphics testing...\n");
    test_gl();
    printf("Graphics testing finished.\n");
}
