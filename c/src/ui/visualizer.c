#include "visualizer.h"

void vis_init(vis_t* vis) {
    vis->vtable[CALL_INIT](vis);
}

void vis_update(vis_t* vis) {
    vis->vtable[CALL_UPDATE](vis);
}