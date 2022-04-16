#ifndef RUNNER_H
#define RUNNER_H

#include "game/game.h"
#include "ui/visualizer.h"

typedef struct {
    game_t* game;
    vis_t* vis;
} runner_t;

runner_t*   runner_create(game_t* game, vis_t* vis);
void        free_runner(runner_t* runner);

void        runner_start(const runner_t* runner);
void        runner_pause(const runner_t* runner);
void        runner_resume(const runner_t* runner);
void        runner_stop(const runner_t* runner);

#endif