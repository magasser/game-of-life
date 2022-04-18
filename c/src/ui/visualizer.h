#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "../game/game.h"

typedef struct {
    void (**vtable)();
    game_t* game;
} vis_t;

enum {
    CALL_INIT,
    CALL_UPDATE,
    CALL_TERMINATE
};

void    vis_init(vis_t* vis);
void    vis_update(vis_t* vis);
void    vis_terminate(vis_t* vis);

#endif