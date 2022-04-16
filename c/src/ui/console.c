#include <stdlib.h>
#include <stdio.h>

#include "console.h"

void console_init(console_t* vis);
void console_update(console_t* vis);

void (*console_vtable[])() = { 
    [CALL_INIT] = console_init,
    [CALL_UPDATE] = console_update,
};

void console_init(console_t* vis) {
    char* out = to_string(vis->game);

    printf("Init %s\n", out);

    free(out);
}

void console_update(console_t* vis) {
    char* out = to_string(vis->game);

    printf("%s\n", out);

    free(out);
}

console_t* console_create(game_t* game) {
    console_t* console = malloc(sizeof(console_t));

    console->vtable = console_vtable;
    console->game = game;

    return console;
}

void free_console(console_t* console) {
    free(console);
}