#ifndef _LCOM_SPRITES_H_
#define _LCOM_SPRITES_H_
#include <lcom/lcf.h>
#include "Controlers/Graphic/graphics.h"

/**
 * @struct Sprite
 * @brief Structure representing a sprite.
 */
typedef struct {
    uint16_t height;
    uint16_t width;
    uint32_t *colors;
    uint32_t color;
    uint8_t pressed;
} Sprite; 

/**
 * @brief Function to create a sprite from an XPM file.
 * @param sprite The XPM map representing the sprite.
 * @return A pointer to the created Sprite struct, or NULL on failure.
 */
Sprite *create_sprite_xpm(xpm_map_t sprite);

/**
 * @brief Function to draw a sprite on the screen.
 * @param sprite Pointer to the sprite to be drawn.
 * @param x X-coordinate of the top-left corner of the sprite.
 * @param y Y-coordinate of the top-left corner of the sprite.
 * @return 0 on success, 1 on failure.
 */
int draw_sprite_xpm(Sprite *sprite, int x, int y);

/**
 * @brief Function to destroy a sprite and free its memory.
 * @param sprite Pointer to the sprite to be destroyed.
 */
void destroy_sprite(Sprite *sprite);

/**
 * @brief Function to create a rectangular sprite with a specified color.
 * @param width Width of the rectangular sprite.
 * @param height Height of the rectangular sprite.
 * @param color Color of the rectangular sprite.
 * @return A pointer to the created Sprite struct, or NULL on failure.
 */
Sprite *create_rectangle_sprite(int width, int height, uint32_t color);


/**
 * @brief Function to draw a sprite on the screen, changing white pixels to a specified color.
 * @param sprite Pointer to the sprite to be drawn.
 * @param x The x-coordinate on the screen where the sprite should be drawn.
 * @param y The y-coordinate on the screen where the sprite should be drawn.
 * @return 0 on success, 1 on failure.
 */
int draw_white_sprite(Sprite *sprite, int x, int y);
#endif
