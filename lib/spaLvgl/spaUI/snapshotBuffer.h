#ifndef SNAPSHOTBUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "lvgl.h"

lv_draw_buf_t * snapshot_create_draw_buf(lv_obj_t * obj, lv_color_format_t cf);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif