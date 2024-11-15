#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include "Controlers/Mouse/KBC.h"
#include "Controlers/Timers/i8042.h"
#include <lcom/lcf.h>
#include <stdint.h>

typedef struct {
  bool right_click;
  bool left_click;
  int16_t x;
  int16_t y;
} MouseInfo;

int(mouse_subscribe_int)(uint8_t *bit_no);

int(mouse_unsubscribe_int)();

void(mouse_ih)();

void(mouse_sync_bytes)();

void(mouse_set_info)();

int(mouse_write)(uint8_t command);

int set_sample_rate(uint8_t rate);

#endif
