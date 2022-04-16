#include <stdlib.h>
#include <stdio.h>

#include "console.h"

#define CHAR_DEAD   'o'
#define CHAR_ALIVE  '#'

void console_init(console_t* vis);
void console_update(console_t* vis);

void (*console_vtable[])() = { 
    [CALL_INIT] = console_init,
    [CALL_UPDATE] = console_update,
};

void print_game(const game_t* game);
void clear_console(void);

void console_init(console_t* vis) {
    print_game(vis->game);
}

void console_update(console_t* vis) {
    print_game(vis->game);
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

void print_game(const game_t* game) {
    char* out = to_string(game);
    printf("%s\n", out);
    free(out);

    char state;
    cell_t cell;
    for (uint32_t y = 0; y < game->height; ++y) {
        for (uint32_t x = 0; x < game->width; ++x) {
            cell.x = x;
            cell.y = y;

            state = is_alive(game, cell) ? CHAR_ALIVE : CHAR_DEAD;

            putchar(state);

            if (x < game->width - 1) {
                putchar(' ');
            }
        }
        putchar('\n');
    }
}

void clear_console(void) {
    printf("\e[1;1H\e[2J");
}