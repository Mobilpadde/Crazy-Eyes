#include <pebble.h>
#include <config.h>

static Window *mainWindow;
static Layer *smileLayer, *scleraLeftLayer, *scleraRightLayer, *corneaLeftLayer, *corneaRightLayer;
int hours = 0, minutes = 0;

void updateTime(void){
	time_t tmp = time(NULL);
	struct tm *tick = localtime(&tmp);
	
	int hoursBefore = hours;
	int minutesBefore = minutes;
	
	hours = tick->tm_hour;
	minutes = tick->tm_min;
	
	if(hoursBefore != hours) layer_mark_dirty(scleraLeftLayer);
	if(minutesBefore != minutes) layer_mark_dirty(scleraRightLayer);
}

void tickHandler(struct tm *tick, TimeUnits unitsChanged){
	updateTime();
}

void drawEye(Layer *layer, GContext *ctx, int time, int divBy){
	float angle = (time * 360) / divBy;
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Well...");
	
	graphics_context_set_antialiased(ctx, true);
	graphics_context_set_fill_color(ctx, SCLERA_COLOR);
	graphics_fill_circle(ctx, GPoint(SCLERA_RADIUS, SCLERA_RADIUS), SCLERA_RADIUS);
	
	// Do cornea current time use time and divBy
	GRect bounds = layer_get_bounds(layer);
	GRect frame = grect_inset(bounds, GEdgeInsets(SCLERA_RADIUS * CORNEA_MULTIPLIER));
	GPoint pos = gpoint_from_polar(frame, GOvalScaleModeFillCircle, DEG_TO_TRIGANGLE(angle));
	
	graphics_context_set_antialiased(ctx, true);
	graphics_context_set_fill_color(ctx, CORNEA_COLOR);
	graphics_fill_circle(ctx, pos, SCLERA_RADIUS * CORNEA_MULTIPLIER);
}

void leftEyeProcedure(Layer *layer, GContext *ctx){
	drawEye(layer, ctx, hours, 12);
}

void rightEyeProcedure(Layer *layer, GContext *ctx){
	drawEye(layer, ctx, minutes, 60);
}

void smileProcedure(Layer *layer, GContext *ctx){
	GRect bounds = layer_get_bounds(layer);
	
	graphics_context_set_antialiased(ctx, true);
	graphics_context_set_stroke_color(ctx, SCLERA_COLOR);
	graphics_fill_radial(ctx, bounds, GOvalScaleModeFillCircle, 10, DEG_TO_TRIGANGLE(90), DEG_TO_TRIGANGLE(270));
}

void mainWindowLoad(Window *window){
	window_set_background_color(window, BACKGROUND_COLOR);
	
	Layer *mainLayer = window_get_root_layer(window);
	
	GRect leftBounds = GRect(EYE_INDENT, EYE_INDENT, SCLERA_RADIUS * 2, SCLERA_RADIUS * 2);
	GRect rightBounds = GRect(WIDTH - EYE_INDENT - SCLERA_RADIUS * 2, EYE_INDENT, SCLERA_RADIUS * 2, SCLERA_RADIUS * 2);
	
	scleraLeftLayer = layer_create(leftBounds);
	scleraRightLayer = layer_create(rightBounds);
	
	corneaLeftLayer = layer_create(layer_get_bounds(scleraLeftLayer));
	corneaRightLayer = layer_create(layer_get_bounds(scleraRightLayer));
	
	layer_add_child(mainLayer, scleraLeftLayer);
	layer_add_child(mainLayer, scleraRightLayer);
	
	layer_add_child(mainLayer, corneaLeftLayer);
	layer_add_child(mainLayer, corneaRightLayer);
	
	layer_set_update_proc(scleraLeftLayer, leftEyeProcedure);
	layer_set_update_proc(scleraRightLayer, rightEyeProcedure);
	
	GRect smileBounds = GRect(EYE_INDENT, HEIGHT - EYE_INDENT - SCLERA_RADIUS * 3.5, SCLERA_RADIUS * 4, SCLERA_RADIUS * 4);
	smileLayer = layer_create(smileBounds);
	layer_set_update_proc(smileLayer, smileProcedure);
	layer_add_child(mainLayer, smileLayer);
}

void mainWindowUnload(Window *window){
	layer_destroy(smileLayer);
	
	layer_destroy(corneaLeftLayer);
	layer_destroy(scleraLeftLayer);
	
	layer_destroy(corneaRightLayer);
	layer_destroy(scleraRightLayer);
}

void init(void){
	mainWindow = window_create();
	
	window_set_window_handlers(mainWindow, (WindowHandlers){
		.load = mainWindowLoad,
		.unload = mainWindowUnload
	});
	
	window_stack_push(mainWindow, true);
	
	updateTime();
	tick_timer_service_subscribe(MINUTE_UNIT, tickHandler);
}

void deinit(void){
	window_destroy(mainWindow);
}

int main(void){
	init();
	app_event_loop();
	deinit();
}