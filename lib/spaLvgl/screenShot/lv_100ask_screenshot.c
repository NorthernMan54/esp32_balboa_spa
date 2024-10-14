/**
 * @file lv_100ask_screenshot.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_100ask_screenshot.h"

#if LV_USE_100ASK_SCREENSHOT != 0

#include "libs/lodepng/lodepng.h"

#define TJE_IMPLEMENTATION
#include "tiny_jpeg.h"

#include "save_as_bmp.h"
#include "save_as_png.h"

/*********************
 *      DEFINES
 *********************/
#define UpAlign4(n) (((n) + 3) & ~3)
#define UpAlign8(n) (((n) + 7) & ~7)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

// static void data_pre_processing(lv_draw_buf_t *snapshot, uint16_t bpp, lv_100ask_screenshot_sv_t screenshot_sv);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
bool lv_100ask_screenshot_create(lv_obj_t *obj, lv_color_format_t cf, lv_100ask_screenshot_sv_t screenshot_sv, const char *filename)
{
    lv_draw_buf_t *snapshot = lv_snapshot_take(obj, cf);

    if (snapshot)
    {
        data_pre_processing(snapshot, LV_COLOR_DEPTH, screenshot_sv);

        if (screenshot_sv == LV_100ASK_SCREENSHOT_SV_PNG)
        {
            if (LV_COLOR_DEPTH == 16)
            {
                save_as_png_file(snapshot->data, snapshot->header.w, snapshot->header.h, 24, filename);
            }
            else if (LV_COLOR_DEPTH == 32)
            {
                save_as_png_file(snapshot->data, snapshot->header.w, snapshot->header.h, 32, filename);
            }
        }
#if LV_USE_100ASK_BMP != 0
        else if (screenshot_sv == LV_100ASK_SCREENSHOT_SV_BMP)
        {

            if (LV_COLOR_DEPTH == 16)
            {
                save_as_bmp_file(snapshot->data, snapshot->header.w, snapshot->header.h, 24, filename);
            }
            else if (LV_COLOR_DEPTH == 32)
            {
                save_as_bmp_file(snapshot->data, snapshot->header.w, snapshot->header.h, 32, filename);
            }
        }
#endif
#if LV_USE_100ASK_TINY_JPEG != 0
        else if (screenshot_sv == LV_100ASK_SCREENSHOT_SV_JPEG)
        {
            if (LV_COLOR_DEPTH == 16)
            {
                tje_encode_to_file(filename, snapshot->header.w, snapshot->header.h, 3, snapshot->data);
            }
            else if (LV_COLOR_DEPTH == 32)
            {
                tje_encode_to_file(filename, snapshot->header.w, snapshot->header.h, 4, snapshot->data);
            }
        }
#endif

        lv_snapshot_free(snapshot);
        return true;
    }

    return false;
}

bool lv_100ask_screenshot_memory(lv_obj_t *obj, lv_color_format_t cf, lv_100ask_screenshot_sv_t screenshot_sv, uint8_t *jpegBuffer, size_t jpegSize)
{
    lv_draw_buf_t *snapshot = lv_snapshot_take(obj, cf);

    if (snapshot)
    {
        log_i("snapshot->width=%u, snapshot->height=%u, snapshot->data_size=%u", snapshot->header.w, snapshot->header.h, snapshot->data_size);

        // data_pre_processing(snapshot, LV_COLOR_DEPTH, screenshot_sv);

        log_i("jpegSize=%u, jpegBuffer=%p", jpegSize, jpegBuffer);

        u_int error = lodepng_encode_memory(&jpegBuffer, &jpegSize,
                                            snapshot->data, snapshot->header.w, snapshot->header.h, 2, 8);

        log_i("jpegSize=%u, jpegBuffer=%p", jpegSize, jpegBuffer);

        lv_snapshot_free(snapshot);

        log_i("lodepng_encode_memory result: %d", error);

        if (error == 0)
        {
            return true;
        }
        else
        {
            log_i("lodepng_encode_memory failed");
            return false;
        }
    }

    log_i("lv_snapshot_take failed");
    return false;
}

/*=====================
 * Other functions
 *====================*/

/**********************
 *   STATIC FUNCTIONS
 **********************/
void data_pre_processing(lv_draw_buf_t *snapshot, uint16_t bpp, lv_100ask_screenshot_sv_t screenshot_sv)
{
    if (bpp == 16)
    {
        uint16_t rgb565_data = 0;
        uint32_t count = 0;
        for (int w = 0; w < snapshot->header.w; w++)
        {
            for (int h = 0; h < snapshot->header.h; h++)
            {
                rgb565_data = (uint16_t)((*(uint8_t *)(snapshot->data + count + 1) << 8) | *(uint8_t *)(snapshot->data + count));
                if ((screenshot_sv == LV_100ASK_SCREENSHOT_SV_PNG) || (screenshot_sv == LV_100ASK_SCREENSHOT_SV_JPEG))
                {
                    *(uint8_t *)(snapshot->data + count) = (uint8_t *)(((rgb565_data) >> 11) << 3);
                    *(uint8_t *)(snapshot->data + count + 1) = (uint8_t *)(((rgb565_data) >> 5) << 2);
                    *(uint8_t *)(snapshot->data + count + 2) = (uint8_t *)(((rgb565_data) >> 0) << 3);
                }
                else if (screenshot_sv == LV_100ASK_SCREENSHOT_SV_BMP)
                {
                    *(uint8_t *)(snapshot->data + count) = (uint8_t *)(((rgb565_data) >> 0) << 3);
                    *(uint8_t *)(snapshot->data + count + 1) = (uint8_t *)(((rgb565_data) >> 5) << 2);
                    *(uint8_t *)(snapshot->data + count + 2) = (uint8_t *)(((rgb565_data) >> 11) << 3);
                }

                count += 3;
            }
        }
    }
    else if (((screenshot_sv == LV_100ASK_SCREENSHOT_SV_PNG) || (screenshot_sv == LV_100ASK_SCREENSHOT_SV_JPEG)) && (bpp == 32))
    {
        uint8_t tmp_data = 0;
        uint32_t count = 0;
        for (int w = 0; w < snapshot->header.w; w++)
        {
            for (int h = 0; h < snapshot->header.h; h++)
            {
                tmp_data = *(snapshot->data + count);
                *(uint8_t *)(snapshot->data + count) = *(snapshot->data + count + 2);
                *(uint8_t *)(snapshot->data + count + 2) = tmp_data;
                count += 4;
            }
        }
    }
}

#endif /*LV_USE_100ASK_SCREENSHOT*/
