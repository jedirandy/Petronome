#include "state.h"
#define SECS_PER_MIN 60
int bpm = 120;
bool is_running = false;

bool toggle() {
    is_running = !is_running;
    return is_running;
}

int get_interval() {
    return 1 / (bpm / (float)SECS_PER_MIN) * 1000;
}