#include <pebble.h>

//#define KEY_TEMPERATURE 0
//#define KEY_CONDITIONS 1

static Window *s_main_window;
static TextLayer *s_hour_layer;
static TextLayer *s_min_layer;
//static TextLayer *s_date_layer;
//static TextLayer *s_sometext_layer; //May the force be with you...
//static TextLayer *s_weather_layer;

static GFont s_hour_font;
static GFont s_min_font;
//static GFont s_sometext_font;
//static GFont s_weather_font;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void update_time() {
	// Get a tm structure
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);

	// Create a long-lived buffer
	static char bufferMin[] = "00";
	static char bufferHour[] = "00";

	//static char bufferDate[] = "Day, Mon 00";
	// Write the current hours and minutes into the buffer
	if(clock_is_24h_style() == true) {
		//Use 2h hour format
		strftime(bufferHour, sizeof("00"), "%H", tick_time);
		strftime(bufferMin, sizeof("00"),"%M",tick_time);
	} else {
		//Use 12 hour format
		strftime(bufferHour, sizeof("00"), "%I", tick_time);
		strftime(bufferMin, sizeof("00"),"%M",tick_time);
	}
	// Display this time on the TextLayer
	text_layer_set_text(s_hour_layer,bufferHour);
	text_layer_set_text(s_min_layer,bufferMin);

}

static void main_window_load(Window *window) {
	//Create GBitmap, then set to created BitmapLayer
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIREWOLF);
	s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 111));
	//bitmap_layer_set_background_color(s_background_layer, GColorWhite);

	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
	

	// Create hour TextLayer
	s_hour_layer = text_layer_create(GRect(0, 111, 72, 168));
	text_layer_set_text(s_hour_layer, "00");
	text_layer_set_background_color(s_hour_layer, GColorBlack);
	text_layer_set_text_color(s_hour_layer, GColorClear);
	
	//s_hour_font = fonts_load_custom_font(resource_get_handle(FONT_KEY_BITHAM_BOLD_42));
	//text_layer_set_font(s_hour_layer, s_hour_font);
	text_layer_set_font(s_hour_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_layer));
	/////////////////////////////////////////////////

	s_min_layer = text_layer_create(GRect(72,111, 72, 168));
	
	text_layer_set_text(s_min_layer, "00");
	text_layer_set_background_color(s_min_layer, GColorBlack);
	text_layer_set_text_color(s_min_layer, GColorClear);
	//s_min_font = fonts_load_custom_font(resource_get_handle(FONT_KEY_BITHAM_BOLD_42));
	//text_layer_set_font(s_min_layer, s_min_font);
	text_layer_set_font(s_min_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text_alignment(s_min_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_min_layer));


	////////////////////
	//s_sometext_layer = text_layer_create(GRect(0,100,144,68));
	//text_layer_set_text(s_sometext_layer,"Live Long and Prosper... \\\\//");
	//text_layer_set_background_color(s_sometext_layer, GColorBlack);
	//text_layer_set_text_color(s_sometext_layer, GColorClear);



	//Apply to TextLayer
	//text_layer_set_font(s_sometext_layer, s_sometext_font);


	//text_layer_set_text_alignment(s_sometext_layer, GTextAlignmentCenter);

	//layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_sometext_layer));

	// Add it as a child layer to the Window's root layer

	// Create temperature Layer
	//s_weather_layer = text_layer_create(GRect(0, 130, 144, 25));
	//  text_layer_set_background_color(s_weather_layer, GColorClear);
	//  text_layer_set_text_color(s_weather_layer, GColorWhite);
	//text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
	//text_layer_set_text(s_weather_layer, "Loading...");

	// Create second custom font, apply it and add to Window
	//s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_20));
	//text_layer_set_font(s_weather_layer, s_weather_font);
	//layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));

	// Make sure the time is displayed from the start
	update_time();
}

static void main_window_unload(Window *window) {
	//Unload GFont
	fonts_unload_custom_font(s_hour_font);

	fonts_unload_custom_font(s_min_font);
	//fonts_unload_custom_font(s_sometext_font);

	//Destroy GBitmap
	gbitmap_destroy(s_background_bitmap);

	//Destroy BitmapLayer
	bitmap_layer_destroy(s_background_layer);

	// Destroy TextLayer
	text_layer_destroy(s_hour_layer);
	text_layer_destroy(s_min_layer);
	//text_layer_destroy(s_sometext_layer);
	//text_layer_destroy(s_date_layer);

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}



static void init() {
	// Create main Window element and assign to pointer
	s_main_window = window_create();

	// Set handlers to manage the elements inside the Window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});

	// Show the Window on the watch, with animated=true
	window_stack_push(s_main_window, true);

	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

}

static void deinit() {
	// Destroy Window
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
