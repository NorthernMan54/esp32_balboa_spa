/**
 * @file lv_100ask_screenshot.h
 *
 */

#ifndef LV_100ASK_SCREENSHOT_H
#define LV_100ASK_SCREENSHOT_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#if LV_USE_100ASK_SCREENSHOT != 0
#include "lvgl.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * @brief lv_100ask_screenshot_sv_t
 * 
 * The type of screenshot file format
 */
typedef enum {
	LV_100ASK_SCREENSHOT_SV_BMP  = 0,
	LV_100ASK_SCREENSHOT_SV_PNG  = 1,
	LV_100ASK_SCREENSHOT_SV_JPEG = 2,
	LV_100ASK_SCREENSHOT_SV_LAST
}lv_100ask_screenshot_sv_t;

/***********************
 * GLOBAL VARIABLES
 ***********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
bool lv_100ask_screenshot_create(lv_obj_t * obj,  lv_color_format_t cf, lv_100ask_screenshot_sv_t screenshot_sv, const char * filename);
bool lv_100ask_screenshot_memory(lv_obj_t *obj, lv_color_format_t cf, lv_100ask_screenshot_sv_t screenshot_sv, unsigned bitdepth, unsigned char ** out, size_t * outsize);

/*=====================
 * Setter functions
 *====================*/

/*=====================
 * Getter functions
 *====================*/

/*=====================
 * Other functions
 *====================*/

/**********************
 *      MACROS
 **********************/

#endif  /*LV_USE_SCREENSHOT*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_SCREENSHOT_H*/
