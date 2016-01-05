#include <pebble.h>

// Let user choose background
#ifndef BACKGROUND_COLOR
#define BACKGROUND_COLOR GColorDarkGray
#endif

#if PBL_PLATFORM_CHALK
#define WIDTH 180
#define HEIGHT 180
#define SCLERA_RADIUS 30
#else
#define WIDTH 144
#define HEIGHT 168
#define SCLERA_RADIUS 24
#endif

#define CORNEA_MULTIPLIER 0.25

#define EYE_INDENT SCLERA_RADIUS

#define SCLERA_COLOR GColorWhite
#define CORNEA_COLOR GColorBlack

#define MOUTH_COLOR GColorBlack