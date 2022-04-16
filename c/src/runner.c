#include <stdlib.h>

#include "runner.h"

#define GEN_DELAY_MS    1000

runner_t* runner_create(const game_t* game, const vis_t* vis) {
    runner_t* r = malloc(sizeof(runner_t));

    r->game = game;
    r->vis = vis;    
}

void runner_start(const runner_t* r) {
    vis_init(r->vis);
    
    while (1) {
        delay(GEN_DELAY_MS);
    }
}

void runner_pause(const runner_t* r) {
    
}

void runner_resume(const runner_t* r) {
    
}

void runner_stop(const runner_t* r) {

}

void free_runner(runner_t* r) {
    free(r);
}