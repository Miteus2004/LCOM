#ifndef REACT_GAME_CONTROLLER_H
#define REACT_GAME_CONTROLLER_H
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gamesprites.h"
#include "Controlers/Timers/timer.h"
#include "Controlers/Keyboard/keyboard.h"
extern int time_react_countdown;  

/**
 * @brief Enum defining the different states of the reaction game.
 */
typedef enum {
    Start_menu,
    Wait_Game,
    Stop_Game,
    Early_Stop,
    Restart_menu
} Reactmode;

/**
 * @brief Function to print the current react mode and draw corresponding sprites.
 * @return 0 on success, 1 on failure.
 */
int print_react_mode();

/**
 * @brief Function to start the reaction game.
 * Initializes the game and sets the initial state to Start_menu.
 * @return 0 on success.
 */
int start_react_game();

/**
 * @brief Function to handle space key press event in the reaction game.
 * Updates the game state based on the current react_mode.
 */
void react_game_space_pressed();

/**
 * @brief Function to start the timer for measuring reaction time.
 * Sets the timer_running flag to true and resets elapsed time and interrupt counter.
 */
void StartTimer();

/**
 * @brief Function to stop the timer and update the best reaction time.
 * Sets the timer_running flag to false, prints the elapsed time, and updates the best reaction time.
 */
void StopTimer();

/**
 * @brief Function to generate a random time for the user to wait.
 * Generates a random countdown time between 2 and 10 seconds.
 * @return 0 on success.
 */
int give_random_time_wait();

#endif // REACT_GAME_CONTROLLER_H
