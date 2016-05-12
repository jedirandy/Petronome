#include "ui.h"

void set_beat(TextLayer* text_layer, int bpm) {
    snprintf(bpm_str, 10, "%d", bpm);
    text_layer_set_text(text_layer, bpm_str);
}
