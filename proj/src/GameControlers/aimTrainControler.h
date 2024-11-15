#ifndef AIMTRAINCONTROLER_H
#define AIMTRAINCONTROLER_H

#include "Controlers/Timers/timer.h"
#include "gamesprites.h"
#include <lcom/lcf.h>
#include <stdlib.h>


/**
 * @brief Structure representing the position of a target.
 */
typedef struct {
  uint16_t x;
  uint16_t y;
} target;

/**
 * @brief Structure representing user data.
 */
typedef struct {
  int lifes;
  int score;
  int high_score_time_attack;
  int high_score_classic;
  uint32_t best_react_timer;
} user;


/**
 * @brief Enumeration representing different game modes.
 */
typedef enum {
  CLASSIC,
  TIME_ATTACK,
  AIM_TRAIN_MENU,
  GAME_OVER_AIM
} AimMode;

/**
 * @brief Starts the aim training game based on the current aim mode.
 * @return 0 on success, non-zero on failure.
 */
int start_aim_game();

/**
 * @brief Generates a random position for the target.
 * @return 0 on success, non-zero on failure.
 */
int give_random_position();

/**
 * @brief Starts the time attack game mode.
 * @return 0 on success, non-zero on failure.
 */
int start_time_attack();

/**
 * @brief Starts the classic game mode.
 * @return 0 on success, non-zero on failure.
 */
int start_classic();

/**
 * @brief Decreases the user's life count.
 * @return 0 on success, non-zero on failure.
 */
int take_life();

/**
 * @brief Handles when a target is hit.
 * @return 0 on success, non-zero on failure.
 */
int target_hit();

/**
 * @brief Handles a target is miss.
 * @return 0 on success, non-zero on failure.
 */
int miss_target();

#endif // AIMTRAINCONTROLER_H
