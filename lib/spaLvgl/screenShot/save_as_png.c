/**
 * @file save_as_bmp.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "save_as_png.h"
#include "libs/lodepng/lodepng.h"

#if LV_USE_100ASK_SCREENSHOT != 0

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

bool save_as_png_file(uint8_t *image, uint32_t w, uint32_t h, uint32_t bpp, char *filename)
{
  log_i("save_as_png_file %s, width: %lu, height: %lu, bpp: %lu", filename, w, h, bpp);
  if (bpp == 32)
  {
    lodepng_encode32_file(filename, image, w, h);
  }
  else if (bpp == 24)
  {
    lodepng_encode24_file(filename, image, w, h);
  }
}

/*=====================
 * Other functions
 *====================*/

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /*LV_USE_100ASK_SCREENSHOT*/
