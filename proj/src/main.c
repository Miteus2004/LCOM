#include "Controlers/Graphic/graphics.h"
#include "Controlers/Keyboard/keyboard.h"
#include "Controlers/Mouse/mouse.h"
#include "Controlers/RTC/rtc.h"
#include "Controlers/Timers/timer.h"
#include "GameControlers/aimTrainControler.h"
#include "GameControlers/buffering.h"
#include "GameControlers/gamehandlers.h"
#include "GameControlers/gamesprites.h"
#include "GameControlers/statecontroler.h"
#include <lcom/lcf.h>

extern int next_target_sec;
extern GameState state;
extern AimMode aim_mode;
extern int timer_interrupts;
extern bool timer_running;
extern uint32_t elapsed_time_ms;

uint8_t irq_set_TIMER, irq_set_KBC, irq_set_MOUSE, irq_set_RTC;
int fps = 60;
int mouse_rate = 40;
bool Otimized = false;

/**
 * This module serves as the entry point for the application. It orchestrates the setup, execution, 
 * and cleanup of the game loop.
*/


/**
 * @brief Main function, serves as the entry point for the application.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 on success, 1 on failure.
 */
int main(int argc, char **argv) {
  /* main loop */
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/src/Debug/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/src/Debug/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

/**
 * @brief Set up the initial configuration for the game.
 * @return 0 on success, 1 on failure.
 */
int set_up() {
  // uint8_t timer=0;
  // uint32_t freq = 60;
  // if (timer_set_frequency(timer,freq) != 0) {
  //   printf("timer_set_frequency failed\n");
  //   return 1;
  // }
  // gives error pagefault

  if (ret_video(0x14C) != 0) {
    printf("ret_video failed\n");
    return 1;
  }
  printf("ret_video success\n");
  if (set_frame_buffers(0x14C) != 0) {
    printf("set_frame_buffers failed\n");
    return 1;
  }
  printf("set_frame_buffers success\n");

  // Activate data reporting
  if (mouse_write(ENABLE_DATA_REPORT) != 0) {
    printf("Activate data reporting failed\n");
    return 1;
  }

  if (set_sample_rate(mouse_rate) != 0) {
    printf("set_sample_rate failed\n");
    return 1;
  }

  // subscribe
  if (keyboard_subscribe_int(&irq_set_KBC) != 0) {
    printf("keyboard_subscribe_int failed\n");
    return 1;
  }
  if (timer_subscribe_int(&irq_set_TIMER) != 0) {
    printf("timer_subscribe_int failed\n");
    return 1;
  }
  if (mouse_subscribe_int(&irq_set_MOUSE) != 0) {
    printf("mouse_subscribe_int failed\n");
    return 1;
  }
  if (rtc_subscribe_int() != 0) {
    printf("rtc_subscribe_int failed\n");
    return 1;
  }

  set_sprites();

  return 0;
}

/**
 * @brief Clean up resources and exit the game.
 * @return 0 on success, 1 on failure.
 */
int exit_run() {

  if (vg_exit() != 0) {
    printf("set_text_mode failed\n");
    return 1;
  }

  // unsubscribe
  if (keyboard_unsubscribe_int() != 0) {
    printf("keyboard_unsubscribe_int failed\n");
    return 1;
  }
  if (timer_unsubscribe_int() != 0) {
    printf("timer_unsubscribe_int failed\n");
    return 1;
  }
  if (mouse_unsubscribe_int() != 0) {
    printf("mouse_unsubscribe_int failed\n");
    return 1;
  }
  if (rtc_unsubscribe_int() != 0) {
    printf("rtc_unsubscribe_int failed\n");
    return 1;
  }
  // Deactivate data reporting
  if (mouse_write(DISABLE_DATA_REPORT) != 0)
    return 1;

  return 0;
}

/**
 * @brief Main game loop.
 * @return 0 on success, 1 on failure.
 */
int run_game() {
  int ipc_status;
  message msg;

  draw_new_frame();

  while (state != EXIT) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_KBC) {
            if (keyboard_handler() != 0) {
              printf("keyboard_handler failed\n");
              if (exit_run() != 0) {
                printf("exit_run failed\n");
                return 1;
              }
              return 1;
            }
          }
          if (msg.m_notify.interrupts & BIT(irq_set_TIMER)) {
            timer_interrupts++;
            if (timer_running) {
              elapsed_time_ms += 1000 / 60; // Increment elapsed time by ~16.67 ms
            }

            if ((state == AIMGAME) && (aim_mode == CLASSIC) && (next_target_sec > 0)) {
              next_target_sec--;
            }
            if (timer_interrupts % 60 == 0) {
              if (timer_handler() != 0) {
                printf("timer_handler failed\n");
                if (exit_run() != 0) {
                  printf("exit_run failed\n");
                  return 1;
                }
                return 1;
              }
            }
            if (timer_interrupts % (60 / fps) == 0) {
              if (draw_new_frame() != 0) {
                printf("draw_new_frame failed\n");
                return 1;
              }
            }
          }
          if (msg.m_notify.interrupts & irq_set_MOUSE) {
            if (mouse_handler() != 0) {
              printf("mouse_handler failed\n");
              if (exit_run() != 0) {
                printf("exit_run failed\n");
                return 1;
              }
              return 1;
            }
          }

          if (msg.m_notify.interrupts & RTC_MASK) {
            if (rtc_handler() != 0) {
              printf("rtc_handler failed\n");
              if (exit_run() != 0) {
                printf("exit_run failed\n");
                return 1;
              }
              return 1;
            }
          }
      }
    }
  }
  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  if (set_up() != 0) {
    printf("set_up failed\n");
    if (exit_run() != 0) {
      printf("exit_run failed\n");
      return 1;
    }
    return 1;
  }
  state = startgame();

  if (run_game() != 0) {
    printf("run_game failed\n");
    if (exit_run() != 0) {
      printf("exit_run failed\n");
      return 1;
    }
    return 1;
  }
  printf("run finished\n");
  if (exit_run() != 0) {
    printf("exit_run failed\n");
    return 1;
  }
  printf("Exited successfully\n");
  return 0;
}
