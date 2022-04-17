#include <stdarg.h>
#include <stdio.h>

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

void printf_err(const char* message, ...) {
    va_list args;
    static char* err_str = "ERROR: ";

    printf(err_str);
    
    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    printf("\n");
}