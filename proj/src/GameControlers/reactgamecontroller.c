#include "reactgamecontroller.h"
extern vbe_mode_info_t modeinf;
extern int timer_interrupts;
extern bool timer_running;
extern uint32_t elapsed_time_ms;
Reactmode react_mode;
extern int timer_react_countdown;
extern user usr;
bool first_time = true;

/**
 * The React Game Controller module is responsible for managing the state and flow of a  
 * reaction-based game. It initiates the game, updates game states, and manages 
 * the timer functionality. The module contains functions to start the game, switch between 
 * different game states (such as start menu, waiting for user input, early stop, and stop game), 
 * and draw appropriate sprites based on the current state. It also includes functionalities to 
 * start and stop a timer, which measures the user's reaction time and updates the best reaction
 * time for the user. This module represents approximately 22% of the overall project.
 */

int start_react_game() {
  printf("Starting react game\n");
  if (clearscreen() != 0) {
    printf("clearscreen failed\n");
    return 1;
  }
  if (draw_title_score_react() != 0) {
    printf("draw_title_score_react failed\n");
    return 1;
  }
  react_mode = Start_menu;
  return 0;
}

int(print_react_mode)() {
  // if (clearscreen() != 0) {
  //   printf("clearscreen failed\n");
  //   return 1;
  // }
  switch (react_mode) {
    case Start_menu:
      // Draw the start menu sprite
      if (draw_sprite_xpm(react_Start, 0, 0) != 0) {
        printf("Drawing react_Start failed\n");
        return 1;
      }
      break;
    case Wait_Game:
      // Draw the wait game sprite
      if (draw_sprite_xpm(react_Wait, 0, 0) != 0) {
        printf("Drawing react_Wait failed\n");
        return 1;
      }
      break;
    case Early_Stop:
      // Draw the early stop sprite
      if (draw_sprite_xpm(react_ToSoon, 0, 0) != 0) {
        printf("Drawing react_ToSoon failed\n");
        return 1;
      }
      break;
    case Stop_Game: // green
      if (!timer_running) {
        StartTimer();
      }
      printf("Stop_Game\n");
      // Draw the stop game sprite
      if (draw_sprite_xpm(react_Click, 0, 0) != 0) {
        printf("Drawing react_Click failed\n");
        return 1;
      }
      break;
    case Restart_menu:
      StopTimer();
      // Draw the restart menu sprite
      if (draw_sprite_xpm(react_Restart, 0, 0) != 0) {
        printf("Drawing react_Restart failed\n");
        return 1;
      }
      printf("%d\n", elapsed_time_ms);

      break;
    default:
      printf("Invalid react_mode\n");
      return 1;
  }
  return 0;
}

void react_game_space_pressed() {
  // Handle space key press based on the current react_mode
  switch (react_mode) {
    case Start_menu:
      give_random_time_wait();
      react_mode = Wait_Game;
      break;
    case Wait_Game:
      react_mode = Early_Stop;
      break;
    case Early_Stop:
      react_mode = Restart_menu;
      break;
    case Stop_Game:
      react_mode = Restart_menu;
      break;
    case Restart_menu:
      give_random_time_wait();
      react_mode = Wait_Game;
      break;
    default:
      break;
  }
  printf("New mode: %d\n", react_mode);
}

void StartTimer() {
  printf("Starting timer\n");
  timer_running = true;
  elapsed_time_ms = 0;  // Reset elapsed time
  timer_interrupts = 0; // Reset the interrupt counter
}

void StopTimer() {
  timer_running = false;
  if (first_time) {
    printf("First time\n");
    usr.best_react_timer = elapsed_time_ms;
    first_time = false;
  }
  else if (elapsed_time_ms < usr.best_react_timer) {
    usr.best_react_timer = elapsed_time_ms;
  }
}

int give_random_time_wait() {
  timer_react_countdown = (rand() % (10 - 2 + 1)) + 2; // Generate a random number between 2 and 10
  return 0;
}
