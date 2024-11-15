#ifndef STATECONTROLER_H
#define STATECONTROLER_H

#include <lcom/lcf.h>
#include "GameControlers/gamesprites.h"
#include "GameControlers/reactgamecontroller.h"

/**
 * @brief Enumeration for the different states the game can be in.
 * This enumeration defines the possible states the game can transition
 * between, such as the main menu, reaction game, aiming game, game over,
 * scoreboard, and exit.
 */
typedef enum  {
    MAINMENU,
    REACTGAME,
    AIMGAME,
    GAME_OVER,
    SCOREBOARD,
    EXIT
}GameState;

GameState state;

/**
 * @brief Initializes the game to the main menu state.
 * This function sets up the main menu and transitions the game to the MAINMENU state.
 * @return The new state of the game (MAINMENU or EXIT if an error occurs).
 */
GameState startgame();

/**
 * @brief Transitions the game to the reaction game state.
 * This function sets up the reaction game and transitions the game to the REACTGAME state.
 * @return The new state of the game (REACTGAME or EXIT if an error occurs).
 */
GameState reactgame();

/**
 * @brief Transitions the game to the aiming game state.
 * This function sets the aiming mode and transitions the game to the AIMGAME state.
 * @return The new state of the game (AIMGAME or EXIT if an error occurs).
 */
GameState aimgame();

/**
 * @brief Handles the game over state.
 * This function transitions the game to the GAME_OVER state.
 * @return The new state of the game (GAME_OVER).
 */
GameState gameover();

/**
 * @brief Displays the scoreboard and transitions to the scoreboard state.
 * This function handles the logic for displaying the scoreboard and transitions the game to the 
 * SCOREBOARD state.
 * @return The new state of the game (SCOREBOARD).
 */
GameState scoreboard();

#endif // STATECONTROLER_H
