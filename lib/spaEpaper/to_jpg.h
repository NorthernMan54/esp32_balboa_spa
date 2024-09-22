#ifndef TO_JPG_H
#define TO_JPG_H

typedef enum {
    NEW_PIXFORMAT_RGB565,    // 2BPP/RGB565
    NEW_PIXFORMAT_YUV422,    // 2BPP/YUV422
    NEW_PIXFORMAT_YUV420,    // 1.5BPP/YUV420
    NEW_PIXFORMAT_GRAYSCALE, // 1BPP/GRAYSCALE
    NEW_PIXFORMAT_JPEG,      // JPEG/COMPRESSED
    NEW_PIXFORMAT_RGB888,    // 3BPP/RGB888
    NEW_PIXFORMAT_RAW,       // RAW
    NEW_PIXFORMAT_RGB444,    // 3BP2P/RGB444
    NEW_PIXFORMAT_RGB555,    // 3BP2P/RGB555
    NEW_PIXFORMAT_GRAYSCALE16, // .5 BPP/GRAYSCALE 16
} newpixformat_t;

bool newfmt2jpg(uint8_t *src, size_t src_len, uint16_t width, uint16_t height, newpixformat_t format, uint8_t quality, uint8_t ** out, size_t * out_len);

#endif