#include <pebble.h>
#include "state.h"
#include "ui.h"
static Window *window;
static TextLayer *text_layer;
static AppTimer* timer;

void handler(void* data) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "timer called");
    vibes_short_pulse();
    timer = app_timer_register(get_interval(), handler, NULL);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "clicked select %d", bpm);
    if (toggle()) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "ON at BPM %d", get_interval());
        timer = app_timer_register(get_interval(), handler, NULL);
    }
    else {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "OFF");
        app_timer_cancel(timer);
    }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    bpm++;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "clicked up %d", bpm);
    set_beat(text_layer, bpm);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    bpm--;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "clicked down %d", bpm);
    set_beat(text_layer, bpm);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 50, up_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 50, down_click_handler);
}
static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 40));
  set_beat(text_layer, bpm);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  GFont id_font = fonts_get_system_font(FONT_KEY_LECO_38_BOLD_NUMBERS);
  text_layer_set_font(text_layer, id_font);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
    init();
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
    app_event_loop();
    deinit();
}
