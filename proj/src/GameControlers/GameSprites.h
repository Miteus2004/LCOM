#ifndef GAME_SPRITES_H
#define GAME_SPRITES_H
#include "ComunSprites.h"
#include "Controlers/Graphic/graphics.h"
#include "Controlers/Mouse/mouse.h"
#include "Controlers/RTC/rtc.h"
#include "aimTrainControler.h"
#include "reactgamecontroller.h"
#include "sprites.h"
#include "statecontroler.h"
#include <lcom/lcf.h>

/**
 * @struct PixelColor
 * @brief Represents a pixel with its coordinates and color.
 */
typedef struct {
  uint16_t x, y;
  uint32_t color;
} PixelColor;

/**
 * @brief Sets up the game sprites.
 */
void set_sprites();

/**
 * @brief Clears the screen.
 * @return 0 on success, non-zero otherwise.
 */
int clearscreen();

/**
 * @brief Draws the main menu of the game.
 * @return 0 on success, non-zero otherwise.
 */
int draw_main_menu();

/**
 * @brief Draws the mouse cursor.
 * @return 0 on success, non-zero otherwise.
 */
int draw_mouse();

/**
 * @brief Draws a new frame.
 * @return 0 on success, non-zero otherwise.
 */
int draw_new_frame();

/**
 * @brief Draws the timer for the aim game in time attack mode.
 * @return 0 on success, non-zero otherwise.
 */
int draw_aimgame_TimeAttack_timer();

/**
 * @brief Draws the aim game in time attack mode.
 * @return 0 on success, non-zero otherwise.
 */
int draw_aimgame_TimeAttack();

/**
 * @brief Draws a target.
 * @return 0 on success, non-zero otherwise.
 */
int draw_target();

/**
 * @brief Draws a number at a specified position.
 * @param n The number to draw.
 * @param x The x-coordinate of the position.
 * @param y The y-coordinate of the position.
 * @return 0 on success, non-zero otherwise.
 */
int draw_number(int n, uint16_t x, uint16_t y);

/**
 * @brief Draws a letter at a specified position.
 * @param letter The letter to draw.
 * @param x The x-coordinate of the position.
 * @param y The y-coordinate of the position.
 * @return 0 on success, non-zero otherwise.
 */
int draw_letter(char letter, uint16_t x, uint16_t y);

/**
 * @brief Draws a word starting at a specified position.
 * @param word The word to draw.
 * @param x The x-coordinate of the starting position.
 * @param y The y-coordinate of the starting position.
 * @return 0 on success, non-zero otherwise.
 */
int draw_word(char *word, uint16_t x, uint16_t y);

/**
 * @brief Draws a sequence of numbers starting at a specified position.
 * @param number The number to draw.
 * @param x The x-coordinate of the starting position.
 * @param y The y-coordinate of the starting position.
 * @return 0 on success, non-zero otherwise.
 */
int draw_number_seq(int number, uint16_t x, uint16_t y);

/**
 * @brief Draws the scores in the game.
 * @return 0 on success, non-zero otherwise.
 */
int draw_scores();

/**
 * @brief Draws the number of targets left in the aim game.
 * @return 0 on success, non-zero otherwise.
 */
int draw_targets_left();

/**
 * @brief Draws the aim game in classic mode.
 * @return 0 on success, non-zero otherwise.
 */
int draw_aimgame_Classic();

/**
 * @brief Draws the remaining lives of the player.
 * @return 0 on success, non-zero otherwise.
 */
int draw_lifes();

/**
 * @brief Draws the aim menu.
 * @return 0 on success, non-zero otherwise.
 */
int draw_aim_menu();

/**
 * @brief Draws the real-time clock.
 * @return 0 on success, non-zero otherwise.
 */
int draw_rtc();

/**
 * @brief Saves the background pixels of a sprite at a specified position.
 * @param sprite The sprite whose background pixels are to be saved.
 * @param x The x-coordinate of the position.
 * @param y The y-coordinate of the position.
 */
void save_background_pixels(Sprite *sprite, int x, int y);

/**
 * @brief Restores the background pixels that were saved previously.
 */
void restore_background_pixels();

/**
 * @brief Updates the time attack mode of the aim game.
 * @return 0 on success, non-zero otherwise.
 */
int update_TimeAttack();

/**
 * @brief Updates the classic mode of the aim game.
 * @return 0 on success, non-zero otherwise.
 */
int update_Classic();

/**
 * @brief Updates the real-time clock.
 * @return 0 on success, non-zero otherwise.
 */
int update_rtc();

/**
 * @brief Updates the reaction game.
 * @return 0 on success, non-zero otherwise.
 */
int update_ReactGame();

/**
 * @brief Draws the timer and scores for the reaction game.
 * @return 0 on success, non-zero otherwise.
 */
int draw_timer_scores();

/**
 * @brief Draws the title and score for the reaction game.
 * @return 0 on success, non-zero otherwise.
 */
int draw_title_score_react();

#endif // GAME_SPRITES_H
