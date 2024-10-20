#ifndef SPA_LVGL_H
#define SPA_LVGL_H
#ifdef SPALVGL

void spaLvglSetup();
void spaLvglLoop();



extern uint8_t *jpegBuffer; // Buffer for storing the JPEG output
extern size_t jpegSize;           // Size of the JPEG in memory

size_t captureToJPEG();

#endif
#endif