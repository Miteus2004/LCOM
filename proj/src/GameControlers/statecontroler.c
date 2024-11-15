#include "statecontroler.h"
/**
 * The State Controller module is responsible for managing the overall state and flow of the game. 
 * It provides functions to initialize different game states, such as displaying the main menu, 
 * starting the game of testing the reaction speed, aiming, handling game over scenarios, and 
 * displaying the scoreboard. Each function within the module transitions the game to a specific 
 * state and ensures proper initialization by invoking necessary setup functions. If any setup 
 * function fails, the state transitions to EXIT, signaling an error state. 
 */
extern AimMode aim_mode;
GameState startgame() {
  if (draw_main_menu() != 0) {
    printf("draw_main_menu failed\n");
    return EXIT;
  }
  return MAINMENU;
}

GameState reactgame() {
  if (start_react_game() != 0) {
    printf("start_react_game failed\n");
    return EXIT;
  }
  return REACTGAME;
}

GameState aimgame() {
  aim_mode = AIM_TRAIN_MENU;
  if (start_aim_game() != 0) {
    printf("start_aim_game failed\n");
    return EXIT;
  }
  return AIMGAME;
}

GameState gameover() {
  return EXIT;
}

GameState scoreboard() {
  return EXIT;
}
