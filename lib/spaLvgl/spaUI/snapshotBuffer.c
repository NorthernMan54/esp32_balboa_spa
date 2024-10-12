#include "snapshotBuffer.h"
#include <lvgl.h>
#include <Arduino.h>

#define LV_GLOBAL_DEFAULT() (&lv_global)
#define zero_mem LV_GLOBAL_DEFAULT()->memory_zero
#define default_handlers LV_GLOBAL_DEFAULT()->draw_buf_handlers

int32_t obj_get_ext_draw_size(const lv_obj_t * obj)
{
    if(obj->spec_attr) return obj->spec_attr->ext_draw_size;
    else return 0;
}

void * malloc_zeroed(size_t size)
{
    log_i("allocating %lu bytes", (unsigned long)size);
    if(size == 0) {
        log_i("using zero_mem");
        return &zero_mem;
    }

    void * alloc = heap_caps_calloc(1, size, MALLOC_CAP_DEFAULT);;
    if(alloc == NULL) {
        LV_LOG_INFO("couldn't allocate memory (%lu bytes)", (unsigned long)size);
#if LV_LOG_LEVEL <= LV_LOG_LEVEL_INFO
        lv_mem_monitor_t mon;
        lv_mem_monitor(&mon);
        LV_LOG_INFO("used: %zu (%3d %%), frag: %3d %%, biggest free: %zu",
                    mon.total_size - mon.free_size, mon.used_pct, mon.frag_pct,
                    mon.free_biggest_size);
#endif
        return NULL;
    }

    lv_memzero(alloc, size);

    log_i("allocated at %p", alloc);
    return alloc;
}

lv_draw_buf_t * draw_buf_create_ex(const lv_draw_buf_handlers_t * handlers, uint32_t w, uint32_t h,
                                      lv_color_format_t cf, uint32_t stride)
{
    lv_draw_buf_t * draw_buf = malloc_zeroed(sizeof(lv_draw_buf_t));
    LV_ASSERT_MALLOC(draw_buf);
    if(draw_buf == NULL) return NULL;
    if(stride == 0) stride = lv_draw_buf_width_to_stride(w, cf);

    uint32_t size = lv_calculate_draw_buf_size(w, h, cf, stride);

    void * buf = draw_buf_malloc(handlers, size, cf);
    /*Do not assert here as LVGL or the app might just want to try creating a draw_buf*/
    if(buf == NULL) {
        LV_LOG_WARN("No memory: %"LV_PRIu32"x%"LV_PRIu32", cf: %d, stride: %"LV_PRIu32", %"LV_PRIu32"Byte, ",
                    w, h, cf, stride, size);
        lv_free(draw_buf);
        return NULL;
    }

    draw_buf->header.w = w;
    draw_buf->header.h = h;
    draw_buf->header.cf = cf;
    draw_buf->header.flags = LV_IMAGE_FLAGS_MODIFIABLE | LV_IMAGE_FLAGS_ALLOCATED;
    draw_buf->header.stride = stride;
    draw_buf->header.magic = LV_IMAGE_HEADER_MAGIC;
    draw_buf->data = lv_draw_buf_align(buf, cf);
    draw_buf->unaligned_data = buf;
    draw_buf->data_size = size;
    draw_buf->handlers = handlers;
    return draw_buf;
}

lv_draw_buf_t * draw_buf_create(uint32_t w, uint32_t h, lv_color_format_t cf, uint32_t stride)
{
    return draw_buf_create_ex(&default_handlers, w, h, cf, stride);
}

lv_draw_buf_t * snapshot_create_draw_buf(lv_obj_t * obj, lv_color_format_t cf)
{
    lv_obj_update_layout(obj);
    int32_t w = lv_obj_get_width(obj);
    int32_t h = lv_obj_get_height(obj);
    int32_t ext_size = obj_get_ext_draw_size(obj);
    w += ext_size * 2;
    h += ext_size * 2;
    if(w == 0 || h == 0) return NULL;

    return draw_buf_create(w, h, cf, LV_STRIDE_AUTO);
}