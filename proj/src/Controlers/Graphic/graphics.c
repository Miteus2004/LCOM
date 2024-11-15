#include "graphics.h"

extern bool Otimized;
vbe_mode_info_t modeinf; // declaração da estrutura do LCF no vbe.h
char *vidio_mem;

int(ret_video)(uint16_t mode) {
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));
  reg.intno = 0x10;
  reg.ah = 0x4F;
  reg.al = 0x02;
  reg.bx = mode | BIT(14);
  if (sys_int86(&reg) != 0) {
    printf("Set graphic mode failed\n");
    return 1;
  }
  return 0;
}

int(set_text_mode)() { // same as vg_exit()
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));
  reg.intno = 0x10;
  reg.ax = 0x0003;
  reg.bx = 0x0000;
  if (sys_int86(&reg) != 0) {
    printf("Set text mode failed\n");
    return 1;
  }
  return 0;
}

int(frame_buffer)(unsigned short mode) {
  memset(&modeinf, 0, sizeof(modeinf));
  if (vbe_get_mode_info(mode, &modeinf) != 0) { // função do LCF que retorna a informação do modo de video
    printf("vbe_get_mode_info failed\n");
    return 1;
  }

  unsigned int vram_size = modeinf.XResolution * modeinf.YResolution * ((modeinf.BitsPerPixel + 7) / 8.0);

  struct minix_mem_range address;
  address.mr_base = (phys_bytes) modeinf.PhysBasePtr;
  address.mr_limit = address.mr_base + vram_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &address) != 0) {
    printf("sys_privctl failed\n");
    return 1;
  }

  vidio_mem = vm_map_phys(SELF, (void *) address.mr_base, vram_size);
  return vidio_mem == MAP_FAILED;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  // if (Otimized)
  //   return vg_draw_pixel_with_buff(x, y, color);
  if (x >= modeinf.XResolution || y >= modeinf.YResolution)
    return 1;
  uint8_t BytesPerPixel = ((modeinf.BitsPerPixel + 7) / 8.0);
  uint64_t pixel_pos = (y * modeinf.XResolution + x) * BytesPerPixel;
  return memcpy(vidio_mem + pixel_pos, &color, BytesPerPixel) == 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (Otimized)
    return vg_draw_hline_with_buff(x, y, len, color);
  for (uint16_t i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color))
      return 1;
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (Otimized)
    return vg_draw_rectangle_with_buff(x, y, width, height, color);
  for (uint16_t i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color))
      return 1;
  }
  return 0;
}

int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint16_t h_sise = modeinf.XResolution / no_rectangles;
  uint16_t v_sise = modeinf.YResolution / no_rectangles;

  for (uint8_t i = 0; i < no_rectangles; i++) {
    for (uint8_t j = 0; j < no_rectangles; j++) {
      uint32_t color;
      if (modeinf.MemoryModel == DIRECT_COLOR) {
        uint32_t R = Red(j, step, first);
        uint32_t G = Green(i, step, first);
        uint32_t B = Blue(j, i, step, first);
        color = direct_mode(R, G, B);
      }
      else {
        color = indexed_mode(j, i, step, first, no_rectangles);
      }
      if (vg_draw_rectangle(j * h_sise, i * v_sise, h_sise, v_sise, color)) {
        printf("vg_draw_rectangle failed\n");
        return 1;
      }
    }
  }
  return 0;
}

int direct_mode_raw(unsigned j, unsigned i, uint8_t step, uint32_t first) {

  uint32_t Read_2 = (first + j * step) % (1 << modeinf.RedMaskSize);
  uint32_t Green_2 = (first + i * step) % (1 << modeinf.GreenMaskSize);
  uint32_t Blue_2 = (first + (i + j) * step) % (1 << modeinf.BlueMaskSize);
  uint32_t Red = ((1 << modeinf.RedMaskSize) - 1) & (Read_2 >> modeinf.RedFieldPosition);
  uint32_t Green = ((1 << modeinf.GreenMaskSize) - 1) & (Green_2 >> modeinf.GreenFieldPosition);
  uint32_t Blue = ((1 << modeinf.BlueMaskSize) - 1) & (Blue_2 >> modeinf.BlueFieldPosition);

  return (Red << modeinf.RedFieldPosition) | (Green << modeinf.GreenFieldPosition) | (Blue << modeinf.BlueFieldPosition);
}

int(get_color)(uint32_t color, uint32_t *new_color) {
  uint8_t blue = color & (BIT(modeinf.BlueMaskSize) - 1);
  uint8_t green = color & (BIT(modeinf.GreenMaskSize) - 1);
  uint8_t red = color & (BIT(modeinf.RedMaskSize) - 1);
  *new_color = (blue << modeinf.BlueFieldPosition) | (green << modeinf.GreenFieldPosition) | (red << modeinf.RedFieldPosition);
  return 0;
}

// Funções auxiliares da video_test_pattern()

uint32_t(direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << modeinf.RedFieldPosition) | (G << modeinf.GreenFieldPosition) | (B << modeinf.BlueFieldPosition);
}

uint32_t(indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  return (first + (row * n + col) * step) % (1 << modeinf.BitsPerPixel);
}

uint32_t(Red)(unsigned j, uint8_t step, uint32_t first) {
  return (R(first) + j * step) % (1 << modeinf.RedMaskSize);
}

uint32_t(Green)(unsigned i, uint8_t step, uint32_t first) {
  return (G(first) + i * step) % (1 << modeinf.GreenMaskSize);
}

uint32_t(Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first) {
  return (B(first) + (i + j) * step) % (1 << modeinf.BlueMaskSize);
}

uint32_t(R)(uint32_t first) {
  return ((1 << modeinf.RedMaskSize) - 1) & (first >> modeinf.RedFieldPosition);
}

uint32_t(G)(uint32_t first) {
  return ((1 << modeinf.GreenMaskSize) - 1) & (first >> modeinf.GreenFieldPosition);
}

uint32_t(B)(uint32_t first) {
  return ((1 << modeinf.BlueMaskSize) - 1) & (first >> modeinf.BlueFieldPosition);
}

int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;
  uint32_t *colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);
  for (int h = 0; h < img.height; h++) {
    for (int w = 0; w < img.width; w++) {
      if (vg_draw_pixel(x + w, y + h, colors[h * img.width + w])) {
        printf("vg_draw_pixel failed\n");
        return 1;
      }
    }
  }
  return 0;
}

int(frame_buffer_with_buff)(uint16_t mode, char *frame_buffer) {
  memset(&modeinf, 0, sizeof(modeinf));
  if (vbe_get_mode_info(mode, &modeinf) != 0) { // função do LCF que retorna a informação do modo de video
    printf("vbe_get_mode_info failed\n");
    return 1;
  }

  unsigned int vram_size = modeinf.XResolution * modeinf.YResolution * ((modeinf.BitsPerPixel + 7) / 8.0);

  struct minix_mem_range address;
  address.mr_base = modeinf.PhysBasePtr;
  address.mr_limit = address.mr_base + vram_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &address)) {
    printf("Physical memory allocation error\n");
    return 1;
  }

  frame_buffer = vm_map_phys(SELF, (void *) address.mr_base, vram_size);
  if (frame_buffer == MAP_FAILED) {
    printf("Virtual memory allocation error\n");
    return 1;
  }

  return 0;
}

int vg_draw_pixel_with_buff(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= modeinf.XResolution || y >= modeinf.YResolution)
    return 1;
  uint8_t BytesPerPixel = (modeinf.BitsPerPixel + 7) / 8;
  uint64_t pixel_pos = (y * modeinf.XResolution + x) * BytesPerPixel;

  memcpy(vidio_mem + pixel_pos, &color, BytesPerPixel);
  return 0;
}

int(vg_draw_hline_with_buff)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0; i < len; i++)
    if (vg_draw_pixel_with_buff(x + i, y, color) != 0)
      return 1;
  return 0;
}

int(vg_draw_rectangle_with_buff)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (unsigned i = 0; i < height; i++)
    if (vg_draw_hline_with_buff(x, y + i, width, color) != 0) {
      continue;
    }
  return 0;
}
