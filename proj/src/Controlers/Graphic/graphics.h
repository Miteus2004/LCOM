#ifndef LCOM_GRAPHICS_H
#define LCOM_GRAPHICS_H
#include <lcom/lcf.h>

#define DIRECT_COLOR 0x06
#define INDEXED_COLOR 0x105

int(ret_video)(uint16_t mode);

int(waiting_ESC_key)();

int(set_text_mode)();

int(frame_buffer)(uint16_t mode);

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int(get_color)(uint32_t color, uint32_t *new_color);
int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);
int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

// with buffer
int(frame_buffer_with_buff)(uint16_t mode, char *frame_buffer);
int(vg_draw_pixel_with_buff)(uint16_t x, uint16_t y, uint32_t color);
int(vg_draw_hline_with_buff)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_rectangle_with_buff)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

uint32_t(direct_mode)(uint32_t R, uint32_t G, uint32_t B);
uint32_t(indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);
uint32_t(Red)(unsigned j, uint8_t step, uint32_t first);
uint32_t(Green)(unsigned i, uint8_t step, uint32_t first);
uint32_t(Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first);
uint32_t(R)(uint32_t first);
uint32_t(G)(uint32_t first);
uint32_t(B)(uint32_t first);

#endif // LCOM_GRAPHICS_H //
