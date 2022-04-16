#include "helpers.h"

void delay(int milliseconds) {
    uint32_t pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);

    now = then = clock();

    while ((now - then) < pause) {
        now = clock();
    }
}