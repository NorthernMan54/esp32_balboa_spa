#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "uiSpaShared.h"
#include "../sqlUI/ui.h"

/**
 * @brief adjust objects on the screen based on the screen size
 *
 * @param obj base screen object
 */
void adjustSpaScreen()
{
  if (LV_VER_RES < 480)
  {
    lv_obj_add_flag(ui_uiTemperatureHistory, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_uiHeaterHistory, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_align(ui_uiPump1, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_align(ui_uiPump2, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_align(ui_uiLight1, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_align(ui_uiFilter, LV_ALIGN_BOTTOM_RIGHT);
  }
}

#endif