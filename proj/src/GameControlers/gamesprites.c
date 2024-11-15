#include "gamesprites.h"
#define bac_color 0xde9301
extern vbe_mode_info_t modeinf;
extern GameState state;
extern AimMode aim_mode;
extern target tgt;
extern int timer_counter_aim;
extern int timer_interrupts;
extern char *vidio_mem;
extern Reactmode react_mode;
extern MouseInfo mouse_packet;
extern user usr;
extern int targets_left;
extern bool timer_running;
extern uint32_t elapsed_time_ms;
Reactmode curent_r = -1;
int flag = INT32_MAX;
int c_score = -1;
int c_lifes = 4;
int c_targets = -1;
uint32_t c_enlaps = -1;
uint32_t c_bestreact = -1;
target lst_pos = {0, 0};
bool white_text = false;
PixelColor saved_pixels[21 * 21]; // max mouse size
uint16_t saved_pixels_count = 0;
bool flag_2 = true, flag_3 = true;

int(clearscreen)() {
  if (draw_sprite_xpm(background, 0, 0) != 0) {
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  return 0;
}

void(set_sprites)() {
  // Aim Menu
  aim_menu = create_sprite_xpm((xpm_map_t) aim_menu_xpm);
  // Main Menu
  main_menu = create_sprite_xpm((xpm_map_t) MainMenu_xpm);
  mouse_sprite = create_sprite_xpm((xpm_map_t) mouse_xpm);
  aim_sprite = create_sprite_xpm((xpm_map_t) aim_xpm);
  // numbers
  one_sprite = create_sprite_xpm((xpm_map_t) one_xpm);
  two_sprite = create_sprite_xpm((xpm_map_t) two_xpm);
  three_sprite = create_sprite_xpm((xpm_map_t) three_xpm);
  four_sprite = create_sprite_xpm((xpm_map_t) four_xpm);
  five_sprite = create_sprite_xpm((xpm_map_t) five_xpm);
  six_sprite = create_sprite_xpm((xpm_map_t) six_xpm);
  seven_sprite = create_sprite_xpm((xpm_map_t) seven_xpm);
  eight_sprite = create_sprite_xpm((xpm_map_t) eight_xpm);
  nine_sprite = create_sprite_xpm((xpm_map_t) nine_xpm);
  zero_sprite = create_sprite_xpm((xpm_map_t) zero_xpm);
  two_points_sprite = create_sprite_xpm((xpm_map_t) two_points_xpm);
  equal_sprite = create_sprite_xpm((xpm_map_t) equal_xpm);
  slash_sprite = create_sprite_xpm((xpm_map_t) slash_xpm);
  // letters
  A_sprite = create_sprite_xpm((xpm_map_t) a_xpm);
  B_sprite = create_sprite_xpm((xpm_map_t) b_xpm);
  C_sprite = create_sprite_xpm((xpm_map_t) c_xpm);
  D_sprite = create_sprite_xpm((xpm_map_t) d_xpm);
  E_sprite = create_sprite_xpm((xpm_map_t) e_xpm);
  F_sprite = create_sprite_xpm((xpm_map_t) f_xpm);
  G_sprite = create_sprite_xpm((xpm_map_t) g_xpm);
  H_sprite = create_sprite_xpm((xpm_map_t) h_xpm);
  I_sprite = create_sprite_xpm((xpm_map_t) i_xpm);
  J_sprite = create_sprite_xpm((xpm_map_t) j_xpm);
  K_sprite = create_sprite_xpm((xpm_map_t) k_xpm);
  L_sprite = create_sprite_xpm((xpm_map_t) l_xpm);
  M_sprite = create_sprite_xpm((xpm_map_t) m_xpm);
  N_sprite = create_sprite_xpm((xpm_map_t) n_xpm);
  O_sprite = create_sprite_xpm((xpm_map_t) o_xpm);
  P_sprite = create_sprite_xpm((xpm_map_t) p_xpm);
  Q_sprite = create_sprite_xpm((xpm_map_t) q_xpm);
  R_sprite = create_sprite_xpm((xpm_map_t) r_xpm);
  S_sprite = create_sprite_xpm((xpm_map_t) s_xpm);
  T_sprite = create_sprite_xpm((xpm_map_t) t_xpm);
  U_sprite = create_sprite_xpm((xpm_map_t) u_xpm);
  V_sprite = create_sprite_xpm((xpm_map_t) v_xpm);
  W_sprite = create_sprite_xpm((xpm_map_t) w_xpm);
  X_sprite = create_sprite_xpm((xpm_map_t) x_xpm);
  Y_sprite = create_sprite_xpm((xpm_map_t) y_xpm);
  Z_sprite = create_sprite_xpm((xpm_map_t) z_xpm);
  // target
  target_Sprite = create_sprite_xpm((xpm_map_t) target_xpm);
  target_background = create_rectangle_sprite(100, 100, 0x00000000);
  background = create_rectangle_sprite(modeinf.XResolution, modeinf.YResolution, 0x00000000);
  red_background = create_rectangle_sprite(64, 16, bac_color);
  aim_menu_background = create_rectangle_sprite(64, 16, 0x2c2c2c);
  main_menu_background = create_rectangle_sprite(500, 16, 0x7f7f7f);
  // react game
  // react phases
  black_background = create_rectangle_sprite(160, 16, 0x00000000);
  react_Start = create_sprite_xpm((xpm_map_t) ReactionTime_start);
  react_Wait = create_sprite_xpm((xpm_map_t) ReactionTime_wait);
  react_ToSoon = create_sprite_xpm((xpm_map_t) ReactionTime_tosoon);
  react_Click = create_sprite_xpm((xpm_map_t) ReactionTime_click);
  react_Restart = create_sprite_xpm((xpm_map_t) ReactionTime_restart);
}

/**
 * The Game Sprites Controller module manages graphics rendering, user input, and game logic. It 
 * handles drawing various elements such as numbers, letters, sprites, and rectangles essential for 
 * the game's visual representation and interaction. 
 */


int(draw_main_menu)() {
  flag_2 = true;
  if (draw_sprite_xpm(main_menu, 0, 0) != 0) {
    printf("Main Menu draw failed\n");
    return 1;
  }

  // rtc_info current_time;
  if (update_time_date() != 0) {
    printf("Failed to update time and date\n");
    return 1;
  }

  if (draw_rtc() != 0) {
    printf("Failed to draw rtc\n");
    return 1;
  }

  return 0;
}

int(draw_new_frame)() {
  if (state != REACTGAME) {
    if (draw_mouse() != 0) {
      printf("draw_mouse failed\n");
      return 0;
    }
  }

  switch (state) {
    case MAINMENU:

      // mudar rtc?
      break;
    case AIMGAME:
      switch (aim_mode) {
        case CLASSIC:

          if (update_Classic() != 0) {
            printf("ipdate_Classic failed\n");
            return 1;
          }

          if (lst_pos.x != tgt.x && lst_pos.y != tgt.y) {
            if (lst_pos.x != 0 && lst_pos.y != 0) {
              for (size_t i = 0; i < sizeof(saved_pixels) / sizeof(saved_pixels[0]); i++) {
                saved_pixels[i].color = 0x00000000;
              }
              // restore_background_pixels();
              if (draw_sprite_xpm(target_background, lst_pos.x, lst_pos.y) != 0) {
                printf("vg_draw_xpm failed\n");
                return 1;
              }
              // save_background_pixels(aim_sprite, mouse_packet.x, mouse_packet.y);
            }
            if (draw_target() != 0) {
              printf("draw_target failed\n");
              return 1;
            }
            lst_pos.x = tgt.x;
            lst_pos.y = tgt.y;
          }
          break;
        case TIME_ATTACK:

          if (update_TimeAttack() != 0) {
            printf("update_TimeAttack failed\n");
            return 1;
          }

          if (lst_pos.x != tgt.x && lst_pos.y != tgt.y) {
            if (lst_pos.x != 0 && lst_pos.y != 0) {
              for (size_t i = 0; i < sizeof(saved_pixels) / sizeof(saved_pixels[0]); i++) {
                saved_pixels[i].color = 0x00000000;
              }
              // restore_background_pixels();
              if (draw_sprite_xpm(target_background, lst_pos.x, lst_pos.y) != 0) {
                printf("vg_draw_xpm failed\n");
                return 1;
              }
              // save_background_pixels(aim_sprite, mouse_packet.x, mouse_packet.y);
            }
            if (draw_target() != 0) {
              printf("draw_target failed\n");
              return 1;
            }
            lst_pos.x = tgt.x;
            lst_pos.y = tgt.y;
          }

          break;

        default:
          break;
      }
      break;
    case REACTGAME:

      if (update_ReactGame() != 0) {
        printf("update_ReactGame failed\n");
        return 1;
      }

      break;

    default:
      break;
  }

  return 0;
}

int update_ReactGame() {
  if ((c_enlaps != elapsed_time_ms) || (c_bestreact != usr.best_react_timer)) {
    c_enlaps = elapsed_time_ms;
    c_bestreact = usr.best_react_timer;
    draw_timer_scores();
  }
  if (curent_r != react_mode) {
    printf("Updating React Game\n");
    printf("React Mode: %d\n", react_mode);
    printf("Current R: %d\n", curent_r);
    if (print_react_mode() != 0) {
      printf("print_react_mode failed\n");
      return 1;
    }
    curent_r = react_mode;
  }
  return 0;
}

int(draw_title_score_react)() {
  white_text = true;
  if (draw_word("CURRENT REACT TIME = ", 376, modeinf.YResolution - 250) != 0) { // score
    printf("draw_word failed\n");
    return 1;
  }
  if (draw_word("BEST REACT TIME = ", 400, modeinf.YResolution - 150) != 0) { // high score
    printf("draw_word failed\n");
    return 1;
  }
  white_text = false;
  return 0;
}

int(draw_timer_scores)() {
  if (draw_sprite_xpm(black_background, 376 + 332, modeinf.YResolution - 250) != 0) { // high score
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  if (draw_sprite_xpm(black_background, 400 + 288, modeinf.YResolution - 150) != 0) { // score
    printf("vg_draw_xpm failed\n");
    return 1;
  }

  white_text = true;

  if (draw_number_seq(usr.best_react_timer, 400 + 288, modeinf.YResolution - 150) != 0) { // high score
    printf("draw_number_seq failed\n");
    return 1;
  }

  if (draw_number_seq(elapsed_time_ms, 376 + 332, modeinf.YResolution - 250) != 0) { // score
    printf("draw_number_seq failed\n");
    return 1;
  }

  white_text = false;
  return 0;
}

int(draw_target)() {

  if (draw_sprite_xpm(target_Sprite, tgt.x, tgt.y) != 0) {
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  return 0;
}

int draw_mouse() {

  // if (mouse_packet.x < 0)
  //   mouse_packet.x = 0;
  // if (mouse_packet.x > modeinf.XResolution - mouse_sprite->width) {
  //   mouse_packet.x = modeinf.XResolution - mouse_sprite->width;
  // }

  // if (mouse_packet.y < 0)
  //   mouse_packet.y = 0;
  // if (mouse_packet.y > modeinf.YResolution - mouse_sprite->height) {
  //   mouse_packet.y = modeinf.YResolution - mouse_sprite->height;
  // }
  // static int last_x = 0;
  // static int last_y = 0;

  // // Restore previous background pixels
  // restore_background_pixels();

  // // Save current background pixels
  // save_background_pixels(mouse_sprite, mouse_packet.x, mouse_packet.y);
  // // Draw the mouse sprite at the new position
  // if (draw_sprite_xpm(mouse_sprite, mouse_packet.x, mouse_packet.y) != 0) {
  //   printf("draw_sprite_xpm failed\n");
  //   return 1;
  // }

  switch (state) {
    case AIMGAME:
      if (aim_mode != AIM_TRAIN_MENU) {

        if (mouse_packet.x < 0)
          mouse_packet.x = 0;
        if (mouse_packet.x > modeinf.XResolution - aim_sprite->width) {
          mouse_packet.x = modeinf.XResolution - aim_sprite->width;
        }

        if (mouse_packet.y < 0)
          mouse_packet.y = 0;
        if (mouse_packet.y > modeinf.YResolution - aim_sprite->height) {
          mouse_packet.y = modeinf.YResolution - aim_sprite->height;
        }
        if (flag_3) {
          // Save current background pixels
          save_background_pixels(aim_sprite, mouse_packet.x, mouse_packet.y);
          // Draw the mouse sprite at the new position
          if (draw_sprite_xpm(aim_sprite, mouse_packet.x, mouse_packet.y) != 0) {
            printf("draw_sprite_xpm failed\n");
            return 1;
          }
          flag_3 = false;
        }
        else {
          // Restore previous background pixels
          restore_background_pixels();
          // Save current background pixels
          save_background_pixels(aim_sprite, mouse_packet.x, mouse_packet.y);
          // Draw the mouse sprite at the new position
          if (draw_sprite_xpm(aim_sprite, mouse_packet.x, mouse_packet.y) != 0) {
            printf("draw_sprite_xpm failed\n");
            return 1;
          }
        }
      }
      else {
        if (flag_2) {
          if (mouse_packet.x < 0)
            mouse_packet.x = 0;
          if (mouse_packet.x > modeinf.XResolution - mouse_sprite->width) {
            mouse_packet.x = modeinf.XResolution - mouse_sprite->width;
          }

          if (mouse_packet.y < 0)
            mouse_packet.y = 0;
          if (mouse_packet.y > modeinf.YResolution - mouse_sprite->height) {
            mouse_packet.y = modeinf.YResolution - mouse_sprite->height;
          }

          flag_2 = false;
          // Save current background pixels
          save_background_pixels(mouse_sprite, mouse_packet.x, mouse_packet.y);
          // Draw the mouse sprite at the new position
          if (draw_sprite_xpm(mouse_sprite, mouse_packet.x, mouse_packet.y) != 0) {
            printf("draw_sprite_xpm failed\n");
            return 1;
          }
        }
        else {
          if (mouse_packet.x < 0)
            mouse_packet.x = 0;
          if (mouse_packet.x > modeinf.XResolution - mouse_sprite->width) {
            mouse_packet.x = modeinf.XResolution - mouse_sprite->width;
          }

          if (mouse_packet.y < 0)
            mouse_packet.y = 0;
          if (mouse_packet.y > modeinf.YResolution - mouse_sprite->height) {
            mouse_packet.y = modeinf.YResolution - mouse_sprite->height;
          }

          // Restore previous background pixels
          restore_background_pixels();
          // Save current background pixels
          save_background_pixels(mouse_sprite, mouse_packet.x, mouse_packet.y);
          // Draw the mouse sprite at the new position
          if (draw_sprite_xpm(mouse_sprite, mouse_packet.x, mouse_packet.y) != 0) {
            printf("draw_sprite_xpm failed\n");
            return 1;
          }
        }
      }
      break;
    case MAINMENU:
      if (flag_2) {
        if (mouse_packet.x < 0)
          mouse_packet.x = 0;
        if (mouse_packet.x > modeinf.XResolution - mouse_sprite->width) {
          mouse_packet.x = modeinf.XResolution - mouse_sprite->width;
        }

        if (mouse_packet.y < 0)
          mouse_packet.y = 0;
        if (mouse_packet.y > modeinf.YResolution - mouse_sprite->height) {
          mouse_packet.y = modeinf.YResolution - mouse_sprite->height;
        }

        flag_2 = false;
        // Save current background pixels
        save_background_pixels(mouse_sprite, mouse_packet.x, mouse_packet.y);
        // Draw the mouse sprite at the new position
        if (draw_sprite_xpm(mouse_sprite, mouse_packet.x, mouse_packet.y) != 0) {
          printf("draw_sprite_xpm failed\n");
          return 1;
        }
      }
      else {
        if (mouse_packet.x < 0)
          mouse_packet.x = 0;
        if (mouse_packet.x > modeinf.XResolution - mouse_sprite->width) {
          mouse_packet.x = modeinf.XResolution - mouse_sprite->width;
        }

        if (mouse_packet.y < 0)
          mouse_packet.y = 0;
        if (mouse_packet.y > modeinf.YResolution - mouse_sprite->height) {
          mouse_packet.y = modeinf.YResolution - mouse_sprite->height;
        }

        // Restore previous background pixels
        restore_background_pixels();
        // Save current background pixels
        save_background_pixels(mouse_sprite, mouse_packet.x, mouse_packet.y);
        // Draw the mouse sprite at the new position
        if (draw_sprite_xpm(mouse_sprite, mouse_packet.x, mouse_packet.y) != 0) {
          printf("draw_sprite_xpm failed\n");
          return 1;
        }
      }
      break;
    default:
      if (mouse_packet.x < 0)
        mouse_packet.x = 0;
      if (mouse_packet.x > modeinf.XResolution - mouse_sprite->width) {
        mouse_packet.x = modeinf.XResolution - mouse_sprite->width;
      }

      if (mouse_packet.y < 0)
        mouse_packet.y = 0;
      if (mouse_packet.y > modeinf.YResolution - mouse_sprite->height) {
        mouse_packet.y = modeinf.YResolution - mouse_sprite->height;
      }

      // Restore previous background pixels
      restore_background_pixels();
      // Save current background pixels
      save_background_pixels(mouse_sprite, mouse_packet.x, mouse_packet.y);
      // Draw the mouse sprite at the new position
      if (draw_sprite_xpm(mouse_sprite, mouse_packet.x, mouse_packet.y) != 0) {
        printf("draw_sprite_xpm failed\n");
        return 1;
      }
      break;
  }

  return 0;
}

int(draw_aimgame_TimeAttack_timer)() {

  // Calculate the minutes and seconds from timer_counter_aim
  int minutes = timer_counter_aim / 60;
  int seconds = timer_counter_aim - (minutes * 60);

  int firstDigit = seconds / 10;
  int secondDigit = seconds % 10;

  // printf("Minutes: %d, Seconds: %d\n", minutes, seconds);

  if (draw_number(minutes, 980, 8) != 0) {
    printf("draw_number failed\n");
    return 1;
  }
  if (draw_sprite_xpm(two_points_sprite, 996, 8) != 0) {
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  if (draw_number(firstDigit, 1002, 8) != 0) {
    printf("draw_number failed\n");
    return 1;
  }
  if (draw_number(secondDigit, 1018, 8) != 0) {
    printf("draw_number failed\n");
    return 1;
  }
  return 0;
}

int(draw_aimgame_TimeAttack)() {
  flag_3 = true;
  vg_draw_rectangle(0, 0, modeinf.XResolution, 32, bac_color);                        // upper bar
  vg_draw_rectangle(0, modeinf.YResolution - 32, modeinf.XResolution, 32, bac_color); // lower bar

  if (draw_aimgame_TimeAttack_timer() != 0) { // timer
    printf("draw_aimgame_TimeAttack_timer failed\n");
    return 1;
  }
  if (draw_word("TIME ATTACK", 488, 8) != 0) { // mode name
    printf("draw_word failed\n");
    return 1;
  }
  if (draw_word("HIGH SCORE =", 250, modeinf.YResolution - 24) != 0) { // high score
    printf("draw_word failed\n");
    return 1;
  }
  if (draw_word("SCORE =", 674, modeinf.YResolution - 24) != 0) { // score
    printf("draw_word failed\n");
    return 1;
  }
  return 0;
}

int(draw_scores)() {
  if (draw_sprite_xpm(red_background, 458, modeinf.YResolution - 24) != 0) { // high score
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  if (draw_sprite_xpm(red_background, 802, modeinf.YResolution - 24) != 0) { // score
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  switch (aim_mode) {
    case TIME_ATTACK:
      if (draw_number_seq(usr.high_score_time_attack, 458, modeinf.YResolution - 24) != 0) { // high score
        printf("draw_number_seq failed\n");
        return 1;
      }
      if (draw_number_seq(usr.score, 802, modeinf.YResolution - 24) != 0) { // score
        printf("draw_number_seq failed\n");
        return 1;
      }
      break;
    case CLASSIC:
      if (draw_number_seq(usr.high_score_classic, 458, modeinf.YResolution - 24) != 0) { // high score
        printf("draw_number_seq failed\n");
        return 1;
      }
      if (draw_number_seq(usr.score, 802, modeinf.YResolution - 24) != 0) { // score
        printf("draw_number_seq failed\n");
        return 1;
      }
      break;
    default:
      break;
  }

  return 0;
}

int(update_TimeAttack)() {
  if (flag > timer_counter_aim) {
    flag = timer_counter_aim;
    if (draw_sprite_xpm(red_background, 980, 8) != 0) { // timer
      printf("vg_draw_xpm failed\n");
      return 1;
    }
    if (draw_aimgame_TimeAttack_timer() != 0) {
      printf("draw_aimgame_TimeAttack_timer failed\n");
      return 1;
    }
  }
  if (c_score != usr.score) {
    c_score = usr.score;
    if (draw_scores() != 0) {
      printf("draw_scores failed\n");
      return 1;
    }
  }
  return 0;
}

int(update_Classic)() {
  if (c_score != usr.score) {
    c_score = usr.score;
    if (draw_scores() != 0) {
      printf("draw_scores failed\n");
      return 1;
    }
  }
  if (usr.lifes != c_lifes) {
    c_lifes = usr.lifes;
    if (draw_sprite_xpm(red_background, 288, 8) != 0) { // timer
      printf("vg_draw_xpm failed\n");
      return 1;
    }
    if (draw_lifes() != 0) {
      printf("draw_lifes failed\n");
      return 1;
    }
  }
  if (c_targets != targets_left) {
    c_targets = targets_left;
    if (draw_sprite_xpm(red_background, 962, 8) != 0) { // timer
      printf("vg_draw_xpm failed\n");
      return 1;
    }
    if (draw_targets_left() != 0) {
      printf("draw_targets_left failed\n");
      return 1;
    }
  }
  return 0;
}

int(draw_targets_left)() {
  if (draw_word("TARGETS LEFT = ", 722, 8) != 0) { // targets left
    printf("draw_word failed\n");
    return 1;
  }
  if (draw_number_seq(targets_left, 962, 8) != 0) { // targets left
    printf("draw_number_seq failed\n");
    return 1;
  }
  return 0;
}

int(draw_aimgame_Classic)() {
  flag_3 = true;
  vg_draw_rectangle(0, 0, modeinf.XResolution, 32, bac_color);                        // upper bar
  vg_draw_rectangle(0, modeinf.YResolution - 32, modeinf.XResolution, 32, bac_color); // lower bar

  if (draw_word("CLASSIC", 520, 8) != 0) {
    printf("draw_word failed\n");
    return 1;
  }
  if (draw_word("HIGH SCORE =", 250, modeinf.YResolution - 24) != 0) { // high score
    printf("draw_word failed\n");
    return 1;
  }
  if (draw_word("SCORE =", 674, modeinf.YResolution - 24) != 0) { // score
    printf("draw_word failed\n");
    return 1;
  }

  return 0;
}
int(draw_lifes)() {
  if (draw_number(usr.lifes, 288, 8) != 0) {
    printf("draw_number failed\n");
    return 1;
  }
  return 0;
}
int(draw_number)(int n, uint16_t x, uint16_t y) {
  if (white_text) {
    switch (n) {
      case 0:
        if (draw_white_sprite(zero_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 1:
        if (draw_white_sprite(one_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 2:
        if (draw_white_sprite(two_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 3:
        if (draw_white_sprite(three_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 4:
        if (draw_white_sprite(four_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 5:
        if (draw_white_sprite(five_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 6:
        if (draw_white_sprite(six_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 7:
        if (draw_white_sprite(seven_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 8:
        if (draw_white_sprite(eight_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 9:
        if (draw_white_sprite(nine_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      default:
        printf("Invalid number\n");
        return 1;
        break;
    }
    return 0;
  }
  switch (n) {
    case 0:
      if (draw_sprite_xpm(zero_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 1:
      if (draw_sprite_xpm(one_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 2:
      if (draw_sprite_xpm(two_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 3:
      if (draw_sprite_xpm(three_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 4:
      if (draw_sprite_xpm(four_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 5:
      if (draw_sprite_xpm(five_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 6:
      if (draw_sprite_xpm(six_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 7:
      if (draw_sprite_xpm(seven_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 8:
      if (draw_sprite_xpm(eight_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 9:
      if (draw_sprite_xpm(nine_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    default:
      printf("Invalid number\n");
      return 1;
      break;
  }
  return 0;
}

int(draw_letter)(char letter, uint16_t x, uint16_t y) {
  if (white_text) {
    switch (letter) {
      case 'A':
        if (draw_white_sprite(A_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'B':
        if (draw_white_sprite(B_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'C':
        if (draw_white_sprite(C_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'D':
        if (draw_white_sprite(D_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'E':
        if (draw_white_sprite(E_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'F':
        if (draw_white_sprite(F_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'G':
        if (draw_white_sprite(G_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'H':
        if (draw_white_sprite(H_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'I':
        if (draw_white_sprite(I_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'J':
        if (draw_white_sprite(J_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'K':
        if (draw_white_sprite(K_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'L':
        if (draw_white_sprite(L_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'M':
        if (draw_white_sprite(M_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'N':
        if (draw_white_sprite(N_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'O':
        if (draw_white_sprite(O_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'P':
        if (draw_white_sprite(P_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'Q':
        if (draw_white_sprite(Q_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'R':
        if (draw_white_sprite(R_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'S':
        if (draw_white_sprite(S_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'T':
        if (draw_white_sprite(T_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'U':
        if (draw_white_sprite(U_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'V':
        if (draw_white_sprite(V_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'W':
        if (draw_white_sprite(W_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'X':
        if (draw_white_sprite(X_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'Y':
        if (draw_white_sprite(Y_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case 'Z':
        if (draw_white_sprite(Z_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      case ' ':
        break;
      case '=':
        if (draw_white_sprite(equal_sprite, x, y) != 0) {
          printf("vg_draw_xpm failed\n");
          return 1;
        }
        break;
      default:
        printf("Invalid letter\n");
        return 1;
        break;
    }
    return 0;
  }
  switch (letter) {
    case 'A':
      if (draw_sprite_xpm(A_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'B':
      if (draw_sprite_xpm(B_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'C':
      if (draw_sprite_xpm(C_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'D':
      if (draw_sprite_xpm(D_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'E':
      if (draw_sprite_xpm(E_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'F':
      if (draw_sprite_xpm(F_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'G':
      if (draw_sprite_xpm(G_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'H':
      if (draw_sprite_xpm(H_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'I':
      if (draw_sprite_xpm(I_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'J':
      if (draw_sprite_xpm(J_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'K':
      if (draw_sprite_xpm(K_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'L':
      if (draw_sprite_xpm(L_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'M':
      if (draw_sprite_xpm(M_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'N':
      if (draw_sprite_xpm(N_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'O':
      if (draw_sprite_xpm(O_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'P':
      if (draw_sprite_xpm(P_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'Q':
      if (draw_sprite_xpm(Q_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'R':
      if (draw_sprite_xpm(R_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'S':
      if (draw_sprite_xpm(S_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'T':
      if (draw_sprite_xpm(T_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'U':
      if (draw_sprite_xpm(U_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'V':
      if (draw_sprite_xpm(V_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'W':
      if (draw_sprite_xpm(W_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'X':
      if (draw_sprite_xpm(X_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'Y':
      if (draw_sprite_xpm(Y_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case 'Z':
      if (draw_sprite_xpm(Z_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    case ' ':
      break;
    case '=':
      if (draw_sprite_xpm(equal_sprite, x, y) != 0) {
        printf("vg_draw_xpm failed\n");
        return 1;
      }
      break;
    default:
      printf("Invalid letter\n");
      return 1;
      break;
  }
  return 0;
}

int(draw_word)(char *word, uint16_t x, uint16_t y) {
  for (size_t i = 0; i < strlen(word); i++) {
    if (draw_letter(word[i], x + i * 16, y) != 0) {
      printf("draw_letter failed\n");
      return 1;
    }
  }
  return 0;
}

int(draw_number_seq)(int number, uint16_t x, uint16_t y) {
  char num_str[12];               // Buffer to hold the string representation of the number
  sprintf(num_str, "%d", number); // Convert the number to a string

  for (int i = 0; num_str[i] != '\0'; i++) {
    draw_number(num_str[i] - '0', x + i * 15, y);
  }

  return 0;
}

void(save_background_pixels)(Sprite *sprite, int x, int y) {
  uint16_t height = sprite->height;
  uint16_t width = sprite->width;
  uint32_t current_color;
  saved_pixels_count = 0;
  uint8_t BytesPerPixel = (modeinf.BitsPerPixel + 7) / 8; // Correctly calculate bytes per pixel

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      if (x + w >= modeinf.XResolution || y + h >= modeinf.YResolution)
        continue;

      // Calculate pixel position
      uint64_t pixel_pos = (y + h) * modeinf.XResolution + (x + w);

      // Save the current color
      current_color = 0;
      memcpy(&current_color, vidio_mem + pixel_pos * BytesPerPixel, BytesPerPixel);
      saved_pixels[saved_pixels_count++] = (PixelColor){.x = x + w, .y = y + h, .color = current_color};
    }
  }
}

void(restore_background_pixels)() {
  for (int i = 0; i < saved_pixels_count; i++) {
    vg_draw_pixel(saved_pixels[i].x, saved_pixels[i].y, saved_pixels[i].color);
  }
  saved_pixels_count = 0;
}

int(draw_aim_menu)() {
  flag_2 = true;
  white_text = true;
  if (draw_sprite_xpm(aim_menu, 0, 0) != 0) {
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  if (draw_sprite_xpm(aim_menu_background, 458, modeinf.YResolution - 24) != 0) { // high score
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  if (draw_sprite_xpm(aim_menu_background, 802, modeinf.YResolution - 24) != 0) { // score
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  if (draw_word("SCORE TIME ATTACK =", 138, modeinf.YResolution - 24) != 0) { // high score
    printf("draw_word failed\n");
    return 1;
  }
  if (draw_word("SCORE CLASSIC =", 646, modeinf.YResolution - 24) != 0) { // score
    printf("draw_word failed\n");
    return 1;
  }
  if (draw_number_seq(usr.high_score_time_attack, 458, modeinf.YResolution - 24) != 0) { // high score
    printf("draw_number_seq failed\n");
    return 1;
  }
  if (draw_number_seq(usr.high_score_classic, 902, modeinf.YResolution - 24) != 0) { // score
    printf("draw_number_seq failed\n");
    return 1;
  }
  white_text = false;
  return 0;
}

int(draw_rtc)() {
  int box_x = 374;
  int box_y = 817;
  if (date_time.hours < 10) {
    if (draw_number_seq(0, box_x, box_y) != 0) {
      printf("Failed to draw leading zero for hours\n");
      return 1;
    }
    box_x += 16;
    if (draw_number_seq(date_time.hours, box_x, box_y) != 0) {
      printf("Failed to draw hours\n");
      return 1;
    }
    box_x += 16;
  }
  else {
    if (draw_number_seq(date_time.hours, box_x, box_y) != 0) {
      printf("Failed to draw hours\n");
      return 1;
    }
    box_x += 32;
  }

  if (draw_sprite_xpm(two_points_sprite, box_x, box_y) != 0) {
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  box_x += 16;

  if (draw_number_seq(date_time.minutes, box_x, box_y) != 0) {
    printf("Failed to draw minutes\n");
    return 1;
  }
  box_x += 32;

  if (draw_sprite_xpm(two_points_sprite, box_x, box_y) != 0) {
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  box_x += 16;

  if (draw_number_seq(date_time.seconds, box_x, box_y) != 0) {
    printf("Failed to draw seconds\n");
    return 1;
  }
  box_x += 140;

  if (date_time.day < 10) {
    if (draw_number_seq(0, box_x, box_y) != 0) {
      printf("Failed to draw leading zero for day\n");
      return 1;
    }
    box_x += 16;
    if (draw_number_seq(date_time.day, box_x, box_y) != 0) {
      printf("Failed to draw day\n");
      return 1;
    }
    box_x += 16;
  }
  else {
    if (draw_number_seq(date_time.day, box_x, box_y) != 0) {
      printf("Failed to draw day\n");
      return 1;
    }
    box_x += 32;
  }

  if (draw_sprite_xpm(slash_sprite, box_x, box_y) != 0) {
    printf("Failed to draw slash\n");
    return 1;
  }
  box_x += 10;

  if (date_time.month < 10) {
    if (draw_number_seq(0, box_x, box_y) != 0) {
      printf("Failed to draw leading zero for month\n");
      return 1;
    }
    box_x += 16;
    if (draw_number_seq(date_time.month, box_x, box_y) != 0) {
      printf("Failed to draw month\n");
      return 1;
    }
    box_x += 16;
  }
  else {
    if (draw_number_seq(date_time.month, box_x, box_y) != 0) {
      printf("Failed to draw month\n");
      return 1;
    }
    box_x += 32;
  }

  if (draw_sprite_xpm(slash_sprite, box_x, box_y) != 0) {
    printf("Failed to draw slash\n");
    return 1;
  }
  box_x += 10;

  if (draw_number_seq(2000 + date_time.year, box_x, box_y) != 0) {
    printf("Failed to draw year\n");
    return 1;
  }

  return 0;
}

int(update_rtc)() {
  if (draw_sprite_xpm(main_menu_background, 374, 817) != 0) {
    printf("vg_draw_xpm failed\n");
    return 1;
  }
  if (draw_rtc() != 0) {
    printf("draw_rtc failed\n");
    return 1;
  }
  return 0;
}
