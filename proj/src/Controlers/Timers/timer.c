#include <lcom/lcf.h>
#include "timer.h"

#include <stdint.h>


#include "i8254.h"

int hook_id=0;
int counter=0; 

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */

  if (freq > TIMER_FREQ || freq < 19) return 1;

  uint8_t st;
  timer_get_conf(timer, &st);
  uint8_t cw;
  switch (timer) {
    case 0:
      cw = ((BIT(3) | BIT(2) | BIT(1) | BIT(0)) & st) | TIMER_LSB_MSB | TIMER_SEL0;
      break;
    case 1:
      cw = ((BIT(3) | BIT(2) | BIT(1) | BIT(0)) & st) | TIMER_LSB_MSB | TIMER_SEL1;
      break;
    case 2:
      cw = ((BIT(3) | BIT(2) | BIT(1) | BIT(0)) & st) | TIMER_LSB_MSB | TIMER_SEL2;
      break;
    default:
      return 1;
      break;
  }
  sys_outb(TIMER_CTRL, cw); // sends info to control register (0x43)
  uint16_t div = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  util_get_LSB(div, &lsb);
  util_get_MSB(div, &msb);
  sys_outb(0x40 + timer, lsb);
  sys_outb(0x40 + timer, msb);
  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  /* To be implemented by the students */
  *bit_no = hook_id;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) {
    printf("Failed to set policy for timer interrupt.\n");
    return 1;
  }

  return 0;
}

int(timer_unsubscribe_int)() {
  /* To be implemented by the students */

  if (sys_irqrmpolicy(&hook_id) != 0) {
    printf("Failed to remove policy for timer interrupt.\n");
    return 1;
  }

  return 0;
}

void(timer_int_handler)() {
  /* To be implemented by the students */
  counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */

  uint8_t cw = TIMER_RB_SEL(timer) | TIMER_RB_CMD | TIMER_RB_COUNT_; // ve de alguma forma quais bits a cer testados uma cena assim
  sys_outb(TIMER_CTRL, cw);                                          // sends info to control register (0x43)
  uint8_t timer_port = 0x40 + timer;                                 // timer ports are (0x40,0x41,0x42) so is registor 0x40 + the timer var that can be (0,1,2)
  util_sys_inb(timer_port, st);                                      // reads the configuration of the timer in the timer specific registor
  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) { // i think this comments are correct dk =)
  /* To be implemented by the students */

  union timer_status_field_val conf;

  switch (field) // choses witch task to exacute
  {
    case tsf_all:
      conf.byte = st;
      break;
    case tsf_initial:
      conf.in_mode = (st & TIMER_LSB_MSB) >> 4; // gets the 5th/4th bit of the state, that represents the initial mode
      break;
    case tsf_mode:
      conf.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1; // gets bit 1,2,3 of state, that represent the Programed Mode
      break;
    case tsf_base:
      conf.bcd = st & TIMER_BCD; // gets the last bit of the state, that represents if the counter is binary ou BCB counter
      break;
    default: // non existent task returns 1
      return 1;
      break;
  }
  timer_print_config(timer, field, conf); // given by LCF
  return 0;
}
