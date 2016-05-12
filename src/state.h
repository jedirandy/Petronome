#pragma once
#include <pebble.h>
extern int bpm;
extern bool is_running;

bool toggle(void);
int get_interval(void);
