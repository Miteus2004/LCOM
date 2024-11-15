#include "GameControlers/sprites.h"

/**The Sprite Management module handles the creation, drawing, and destruction of sprites used in 
 * the game. It provides functions to load sprites from XPM files, draw them on the screen, and 
 * manage their memory. This module also includes functionality for creating rectangular sprites 
 * with a specified color. The module is essential for rendering visual elements in the game, 
 * contributing significantly to the graphical representation of game states. 
 */

Sprite *create_sprite_xpm(xpm_map_t sprite) {

  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  if (sp == NULL)
    return NULL;

  xpm_image_t img;
  sp->colors = (uint32_t *) xpm_load(sprite, XPM_8_8_8_8, &img);
  sp->height = img.height;
  sp->width = img.width;

  if (sp->colors == NULL) {
    free(sp);
    return NULL;
  }
  return sp;
}

int draw_sprite_xpm(Sprite *sprite, int x, int y) {
  uint16_t height = sprite->height;
  uint16_t width = sprite->width;
  uint32_t current_color;
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      current_color = sprite->colors[w + h * width];
      if (current_color == 0xFFFFFE)
        continue;
      if (vg_draw_pixel(x + w, y + h, current_color) != 0) {
        printf("vg_draw_pixel failed\n");
        return 1;
      }
    }
  }
  return 0;
}
int draw_white_sprite(Sprite *sprite, int x, int y) {
  uint16_t height = sprite->height;
  uint16_t width = sprite->width;
  uint32_t current_color;
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      current_color = sprite->colors[w + h * width];
      if (current_color == 0xFFFFFE)
        continue;
      current_color = 0xcad2c5;

      if (vg_draw_pixel(x + w, y + h, current_color) != 0) {
        printf("vg_draw_pixel failed\n");
        return 1;
      }
    }
  }
  return 0;
}
void destroy_sprite(Sprite *sprite) {
  if (sprite == NULL)
    return;
  if (sprite->colors)
    free(sprite->colors);
  free(sprite);
  sprite = NULL;
}

Sprite *create_rectangle_sprite(int width, int height, uint32_t color) {
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  if (sp == NULL)
    return NULL;

  sp->height = height;
  sp->width = width;
  sp->colors = (uint32_t *) malloc(width * height * sizeof(uint32_t));
  if (sp->colors == NULL) {
    free(sp);
    return NULL;
  }

  for (int i = 0; i < width * height; i++) {
    sp->colors[i] = color;
  }

  return sp;
}
