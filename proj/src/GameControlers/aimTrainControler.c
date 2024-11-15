#include "aimTrainControler.h"
extern PixelColor saved_pixels[21 * 21];
AimMode aim_mode = AIM_TRAIN_MENU;
target tgt;
int targets_left;
int next_target_sec = 0;
extern vbe_mode_info_t modeinf;
extern int timer_counter_aim;
extern int timer_interrupts;
extern int flag;
user usr = {3, 0, 0, 0, 0};
// 1152 x 864

/**
 * This module controls the gameplay logic for an aiming training program. It includes functions 
 * for starting different game modes such as Time Attack and Classic, handling target hits and 
 * misses, updating scores and lives, and managing the display of the game interface. It also 
 * includes functions for generating random target positions and drawing the game interface. This 
 * module contributes to approximately 22% of the project.
 */

int(start_aim_game)() {

  if (draw_sprite_xpm(background, 0, 0) != 0) {
    printf("draw_sprite_xpm failed\n");
    return 1;
  }

  switch (aim_mode) {
    case AIM_TRAIN_MENU:
      if (clearscreen() != 0) {
        printf("clearscreen failed\n");
        return 1;
      }
      if (draw_aim_menu() != 0) {
        printf("draw_aim_menu failed\n");
        return 1;
      }
      break;
    case TIME_ATTACK:
      // print time attack
      for (size_t i = 0; i < sizeof(saved_pixels) / sizeof(saved_pixels[0]); i++) {
        saved_pixels[i].color = 0x00000000;
      }
      if (start_time_attack() != 0) {
        printf("Failed to start time attack\n");
        return 1;
      }
      break;
    case CLASSIC:
      // print classic
      for (size_t i = 0; i < sizeof(saved_pixels) / sizeof(saved_pixels[0]); i++) {
        saved_pixels[i].color = 0x00000000;
      }
      if (start_classic() != 0) {
        printf("Failed to start classic\n");
        return 1;
      }
      break;
    case GAME_OVER_AIM:
      // print game over
      break;
    default:
      break;
  }
  return 0;
}

int(start_time_attack)() {
  printf("start_time_attack\n");
  usr.score = 0;
  flag = INT32_MAX;
  timer_counter_aim = 60;
  timer_interrupts = 0;
  if (clearscreen() != 0) {
    printf("clearscreen failed\n");
    return 1;
  }
  if (give_random_position(&tgt) != 0) {
    printf("give_random_position failed\n");
    return 1;
  }
  if (draw_aimgame_TimeAttack() != 0) {
    printf("draw_aimgame_TimeAttack failed\n");
    return 1;
  }
  return 0;
}

int(start_classic)() {
  usr.score = 0;
  usr.lifes = 3;
  targets_left = 50;
  // next_target_sec = 0;
  timer_interrupts = 0;
  if (clearscreen() != 0) {
    printf("clearscreen failed\n");
    return 1;
  }
  if (give_random_position(&tgt) != 0) {
    printf("give_random_position failed\n");
    return 1;
  }
  if (draw_aimgame_Classic() != 0) {
    printf("draw_aimgame_Classic failed\n");
    return 1;
  }
  return 0;
}

int give_random_position(target *tgt) {
  uint16_t x = (rand() % (1051 - 1 + 1)) + 1;  // Generate a random number between 1 and 1051
  uint16_t y = (rand() % (732 - 32 + 1)) + 32; // Generate a random number between 32 and 732
  tgt->x = x;
  tgt->y = y;
  return 0;
}

int take_life() {
  if (usr.lifes > 0) {
    usr.lifes--;
  }
  return 0;
}

int target_hit() {
  if (aim_mode == TIME_ATTACK) {
    usr.score++;
    if (usr.score > usr.high_score_time_attack) {
      usr.high_score_time_attack = usr.score;
    }
  }

  if (give_random_position(&tgt) != 0) {
    printf("give_random_position failed\n");
    return 1;
  }

  if (aim_mode == CLASSIC) {
    targets_left--;
    // next_target_sec = 60;
  }

  return 0;
}

int(miss_target)() {
  printf("miss_target\n");
  targets_left--;
  if (give_random_position(&tgt) != 0) {
    printf("give_random_position failed\n");
    return 1;
  }
  printf("miss_target v2\n");
  // next_target_sec = 60;
  return 0;
}
