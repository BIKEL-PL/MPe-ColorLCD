#pragma once

#include <stdbool.h>
#include <stdint.h>

/* -------------------------------------------------------------------------------- */
/* -- CONFIG SECTION                                                             -- */
/* -------------------------------------------------------------------------------- */

//#define USE_MULTITASKING    
/* Enable color mode */
//#define USE_COLOR_RGB888   // RGB = 0xFF,0xFF,0xFF
//#define USE_COLOR_RGB565   // RGB = 0bRRRRRGGGGGGBBBBB

// A special color which means "do not draw", used to let fonts have transparent backgrounds (also to save the cost of rendering when we know the area is already blank)
#define C_TRANSPARENT 0xC1C2

/* Enable needed fonts here */
#define USE_FONT_12X15
#define USE_FONT_15X20
#define USE_FONT_23X30
#define USE_FONT_29X40
//#define USE_FONT_45X60
#define USE_FONT_49X62

#ifdef USE_WITH_560C
#define USE_FONT_94X118
#endif

#ifdef USE_WITH_8X0C
#define USE_FONT_121X150
#endif

#ifdef USE_WITH_8X0C
#define SCREEN_WIDTH 320 // FIXME - merge with DISPLAY_WIDTH
#define SCREEN_HEIGHT 480
#define RIGHT_OFFSET 0
#endif

#ifdef USE_WITH_560C
#define SCREEN_WIDTH 240 // FIXME - merge with DISPLAY_WIDTH
#define SCREEN_HEIGHT 376
#define RIGHT_OFFSET 6 // offset needed becouse housing covers part of display on right side
#endif

// Standard default font selections used most of the time

#define  SMALL_TEXT_FONT              FONT_12X15
#define  CONFIGURATIONS_TEXT_FONT     FONT_15X20
#define  MEDIUM_TEXT_FONT             FONT_23X30

#define  MEDIUM_NUMBERS_TEXT_FONT     FONT_29X40
#define  BIG_NUMBERS_TEXT_FONT        FONT_49X62
#define  LARGE_NUMBERS_TEXT_FONT      FONT_94X118
#define  HUGE_NUMBERS_TEXT_FONT       FONT_121X150



/* Specify platform-dependent integer types here */

#define __UG_FONT_DATA const
typedef uint8_t UG_U8;
typedef int8_t UG_S8;
typedef uint16_t UG_U16;
typedef int16_t UG_S16;
typedef uint32_t UG_U32;
typedef int32_t UG_S32;

/* Example for dsPIC33
 typedef unsigned char         UG_U8;
 typedef signed char           UG_S8;
 typedef unsigned int          UG_U16;
 typedef signed int            UG_S16;
 typedef unsigned long int     UG_U32;
 typedef signed long int       UG_S32;
 */

/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */

/* Feature enablers */
//#define USE_PRERENDER_EVENT
//#define USE_POSTRENDER_EVENT

