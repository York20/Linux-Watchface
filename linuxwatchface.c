#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer, *s_date_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void main_window_load(Window *window){
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  s_background_layer = bitmap_layer_create(bounds);
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_3);
  
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(68, 62), bounds.size.w, 60));
  s_date_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(118, 112), bounds.size.w, 150));
  
  //Date pointer general things
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  //time pointer general things
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  //Render Layers to main Window Container by adding layers as child.
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window){
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  bitmap_layer_destroy(s_background_layer);
}

static void update_time(){
  //Append Time datatbase with value temp calling NULL function to get an automatic value aka current time
  time_t temp = time(NULL);
  
  //Creates a time pointer with the temp value passed to the localtime() function.
  struct tm *t = localtime(&temp);
  
  //Creates memory buffers to store data
  static char time_buffer[] = "00:00:00";
  static char date_buffer[11];
  
  //Writes time and date data to appropriate buffers
  strftime(date_buffer, sizeof(date_buffer), "%a %d %b", t);
  
  if(clock_is_24h_style()) {
    strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", t);
  } else {
    strftime(time_buffer, sizeof(time_buffer), "%I:%M:%S", t);
  }
  
  //Appends calculated data to correspoing Graphical layers
  text_layer_set_text(s_time_layer, time_buffer);
  text_layer_set_text(s_date_layer, date_buffer);
}
 
static void tick_handler(struct tm *t, TimeUnits units_changed){
  update_time();
}

static void init(){
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {.load = main_window_load, .unload = main_window_unload});
  window_stack_push(s_main_window, true);
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
  update_time();
}

static void deinit(){
  window_destroy(s_main_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}
