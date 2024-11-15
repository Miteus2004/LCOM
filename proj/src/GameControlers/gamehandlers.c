#include "gamehandlers.h"
extern GameState state;
extern AimMode aim_mode;
extern uint8_t byte_index;
extern MouseInfo mouse_packet;
extern target tgt;
extern bool Otimized;
extern int targets_left;
extern user usr;
extern int next_target_sec;
extern int fps;
extern bool space_pressed;
extern uint8_t scancode;
extern Reactmode react_mode;
int timer_counter_aim;
int timer_interrupts = 0;
int timer_react_countdown;
uint32_t elapsed_time_ms = 0;
bool timer_running = false;

/**
 * The Game Handlers module manages the core interactions and event handling within the game, 
 * including timer, keyboard, mouse, rtc, and button events. It is responsible for updating the 
 * game state based on user input and timed events, ensuring that the game logic runs smoothly. 
 * This module includes functions to handle different games, such as reaction game and aim 
 * training, and transitions between game modes. 
 */

int timer_handler() {
  if (Otimized)
    swap_buffers();
  switch (state) {
    case AIMGAME:
      switch (aim_mode) {
        case CLASSIC:
          // next_target_sec++;
          usr.score++;

          if (targets_left <= 0) {
            if (usr.score > usr.high_score_classic) {
              usr.high_score_classic = usr.score;
            }
            printf("game over\n");
            state = aimgame();
          }
          if (usr.lifes <= 0) {

            printf("game over\n");
            state = aimgame();
          }
          // else if (next_target_sec <= 0) {
          //   if (miss_target() != 0) {
          //     printf("miss_target failed\n");
          //     return 1;
          //   }
          //   next_target_sec = 60;
          // }
          break;
        case TIME_ATTACK:
          timer_counter_aim--;
          if (timer_counter_aim <= 0) {
            printf("game over\n");
            state = aimgame();
          }
          break;

        default:
          break;
      }
      break;
    case REACTGAME:
      if (react_mode == Wait_Game) {
        timer_react_countdown--;
        if (timer_react_countdown <= 0) {
          react_mode = Stop_Game;
        }
      }
      break;

    default:
      break;
  }

  return 0;
}

int keyboard_handler() {
  kbc_ih();
  if (scancode == PRESS_SPACE) {
    if (state == REACTGAME) {
      react_game_space_pressed();
    }
  }
  if (scancode == BREAK_ESC) {
    if (state == MAINMENU) {
      state = EXIT;
    }
    else if ((state == REACTGAME) || ((state == AIMGAME)&&(aim_mode == AIM_TRAIN_MENU))) {
      state = startgame();
    }
  }
  return 0;
}


int buttons_handler() {
  switch (state) {
    case MAINMENU:
      if (mouse_packet.left_click == 1) {
        // printf("left click\n");
        if (mouse_packet.x >= 353 && mouse_packet.x <= 353 + 461 && mouse_packet.y >= 276 && mouse_packet.y <= 276 + 198) {
          state = reactgame();
          return 0;
        }
        if (mouse_packet.x >= 349 && mouse_packet.x <= 349 + 461 && mouse_packet.y >= 585 && mouse_packet.y <= 585 + 198) {
          state = aimgame();
          return 0;
        }
        return 0;
      }
      break;
    case AIMGAME:
      // printf("left: %d n", mouse_packet.left_click);
      if (mouse_packet.left_click == 1) {

        switch (aim_mode) {
          case CLASSIC:
            if (mouse_packet.x >= tgt.x && mouse_packet.x <= tgt.x + 100 && mouse_packet.y >= tgt.y && mouse_packet.y <= tgt.y + 100) {
              printf("target hit\n");
              if (target_hit() != 0) {
                printf("target_hit failed\n");
                return 1;
              }
            }
            else {
              if (miss_target() != 0) {
                printf("miss_target failed\n");
                return 1;
              }
              printf("take life\n");
              if (take_life() != 0) {
                printf("take_life failed\n");
                return 1;
              }
              printf("lifes: %d\n", usr.lifes);
            }
            return 0;

            break;
          case TIME_ATTACK:
            if (mouse_packet.x >= tgt.x && mouse_packet.x <= tgt.x + 100 && mouse_packet.y >= tgt.y && mouse_packet.y <= tgt.y + 100) {
              printf("target hit\n");
              if (target_hit() != 0) {
                printf("target_hit failed\n");
                return 1;
              }
              return 0;
            }
            break;
          case AIM_TRAIN_MENU:
            if (mouse_packet.x >= 80 && mouse_packet.x <= 80 + 938 && mouse_packet.y >= 235 && mouse_packet.y <= 235 + 104) {
              aim_mode = CLASSIC;
              if (start_classic() != 0) {
                printf("start_classic failed\n");
                return 1;
              }
              return 0;
            }
            if (mouse_packet.x >= 80 && mouse_packet.x <= 80 + 938 && mouse_packet.y >= 460 && mouse_packet.y <= 460 + 104) {
              aim_mode = TIME_ATTACK;
              // printf("start time attack mouse handler\n");
              if (start_time_attack() != 0) {
                printf("start_time_attack failed\n");
                return 1;
              }
              return 0;
            }
            if (mouse_packet.x >= 80 && mouse_packet.x <= 80 + 938 && mouse_packet.y >= 682 && mouse_packet.y <= 682 + 104) {
              state = startgame();
              return 0;
            }
            break;
          default:
            break;
        }
      }
      break;
    default:
      break;
  }
  return 0;
}

int mouse_handler() {
  mouse_ih();
  mouse_sync_bytes();
  if (byte_index == 3) {
    mouse_set_info();
    if (buttons_handler() != 0) {
      printf("buttons_handler failed\n");
      return 1;
    }
    byte_index = 0;
  }
  return 0;
}

int rtc_handler() {
  if (timer_interrupts % fps == 0) {
    if (update_time_date() != 0) {
      printf("update_time_date failed\n");
      return 1;
    }
    if (state == MAINMENU) {
      if (update_rtc() != 0) {
        printf("update_rtc failed\n");
        return 1;
      }
    }
  }
  return 0;
}
