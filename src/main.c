#include <pebble.h>
#include <stdio.h>
#include <stdlib.h>

Window *my_window;
//Window *history_window;
TextLayer *sugar_layer;
TextLayer *saved_layer;
TextLayer *clear_layer;
ScrollLayer *history_scroll;

TextLayer *history_text;
static int WIDTH = 144;
static int MAX_HEIGHT = 1000;

int sugar = 85;
static char sugar_buffer[] = "000";
char dates[50]; 
int sugar_history[50];
int size = 0;
int center_press = true;

void sugar_data_retrieve (int size) {
  for (int i = 0; i < size; i++) {
    snprintf(sugar_buffer, sizeof(sugar_buffer), "%d", sugar_history[i]);
    text_layer_set_text(history_text, sugar_buffer);
  }
}

void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(clear_layer));
  history_scroll = scroll_layer_create GRect(0,0,144,168);
  scroll_layer_set_click_config_onto_window(history_scroll, my_window);
  history_text = text_layer_create(GRect(0, 0, WIDTH, MAX_HEIGHT));
  sugar_data_retrieve(size);
  text_layer_set_background_color(history_text, GColorClear);
  text_layer_set_text_color(history_text, GColorBlack);
  text_layer_set_font(history_text, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(history_text, GTextAlignmentLeft);
  scroll_layer_set_click_config_onto_window(history_scroll, my_window);
  
  
  scroll_layer_add_child(history_scroll, text_layer_get_layer(history_text));
  layer_add_child(window_get_root_layer(my_window), scroll_layer_get_layer(history_scroll));
}

void select_long_click_release_handler() {
  
}

void save_sugar() {
  sugar_history[size++] = sugar;
  text_layer_set_text(saved_layer, "Blood sugar saved!");
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(saved_layer));
}

void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  sugar++;
  snprintf(sugar_buffer, sizeof(sugar_buffer), "%d", sugar);
  text_layer_set_text(sugar_layer, sugar_buffer);
}

void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  sugar--;
  snprintf(sugar_buffer, sizeof(sugar_buffer), "%d", sugar);
  text_layer_set_text(sugar_layer, sugar_buffer);
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  save_sugar();
}

void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT,  700, select_long_click_handler, select_long_click_release_handler);
}

void window_load (Window *my_window) {
  sugar_layer = text_layer_create(GRect(0, 50, 144, 120));
  text_layer_set_background_color(sugar_layer, GColorClear);
  text_layer_set_text_color(sugar_layer, GColorBlack);
  text_layer_set_text_alignment(sugar_layer, GTextAlignmentCenter);
  text_layer_set_font(sugar_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(sugar_layer));
  snprintf(sugar_buffer, sizeof(sugar_buffer), "%d", sugar);
  
  saved_layer = text_layer_create(GRect(0,0,144,168));
  text_layer_set_background_color(saved_layer, GColorWhite);
  text_layer_set_text_color(saved_layer, GColorBlack);
  text_layer_set_text_alignment(saved_layer, GTextAlignmentCenter);
  text_layer_set_font(saved_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(sugar_layer, sugar_buffer);
  
  clear_layer = text_layer_create(GRect(0,0,144,168));
  text_layer_set_background_color(clear_layer, GColorWhite);
}

void window_unload (Window *my_window) {
  text_layer_destroy(sugar_layer);
}

void handle_init(void) {
  my_window = window_create();
  window_set_window_handlers(my_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_set_click_config_provider(my_window, click_config_provider);
  sugar_layer = text_layer_create(GRect(0, 0, 144, 20));
  window_stack_push(my_window, true);
}

void handle_deinit(void) {
  text_layer_destroy(sugar_layer);
  text_layer_destroy(saved_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
