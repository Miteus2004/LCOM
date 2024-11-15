#ifndef GAMEHANDLERS_H
#define GAMEHANDLERS_H

#include <lcom/lcf.h>
#include "GameControlers/statecontroler.h"
#include "Controlers/Keyboard/keyboard.h"
#include "Controlers/Mouse/mouse.h"
#include "Controlers/RTC/rtc.h"
#include "Controlers/Timers/timer.h"
#include "GameControlers/statecontroler.h"
#include "GameControlers/aimTrainControler.h"
#include "buffering.h"

/**
 * @brief Function to handle timer events and update game state accordingly.
 * This function is responsible for managing timed events within the game,
 * such as updating the target appearance in aim training mode or transitioning
 * between game modes based on elapsed time.
 * @return 0 on success, 1 in failure.
 */
int timer_handler();

/**
 * @brief Function to handle keyboard events.
 * This function processes keyboard input events and updates the game state
 * accordingly, such as starting or exiting the game.
 * @return 0 on success, 1 on failure.
 */
int keyboard_handler();

/**
 * @brief Function to handle mouse events.
 * This function processes mouse input events and updates the game state
 * accordingly, such as detecting clicks on game elements like targets.
 * @return 0 on success, 1 on failure.
 */
int mouse_handler();

/**
 * @brief Function to handle RTC (Real-Time Clock) events.
 * This function is responsible for handling RTC events, such as updating
 * the integraded clock displayed on the screen.
 * @return 0 on success, 1 on failure.
 */
int rtc_handler();

#endif // GAMEHANDLERS_H
