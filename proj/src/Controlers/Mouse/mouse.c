#include "mouse.h"

extern vbe_mode_info_t modeinf;
int hook_id_mouse = 3;
MouseInfo mouse_packet;
uint8_t byte_index = 0; // [0..2]
uint8_t mouse_bytes[3]; // bytes apanhados
uint8_t current_byte;

int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id_mouse);
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id_mouse);
}

void(mouse_ih)() {
  if (read_KBC_output(KBC_WRITE_CMD, &current_byte, 1))
    printf("Error in reading byte from mouse\n");
}

void mouse_sync_bytes() {
  if (byte_index == 0 && (current_byte & BIT(3))) {
    mouse_bytes[byte_index] = current_byte;
    byte_index++;
  }
  else if (byte_index > 0) { // recebe os deslocamentos em X e Y
    mouse_bytes[byte_index] = current_byte;
    byte_index++;
  }
}

void(mouse_set_info)() {

  mouse_packet.right_click = mouse_bytes[0] & MOUSE_RB;
  mouse_packet.left_click = mouse_bytes[0] & MOUSE_LB;

  if (mouse_bytes[0] & MOUSE_X_OVERFLOW || mouse_bytes[0] & MOUSE_Y_OVERFLOW)
    return;

  int16_t delta_x = mouse_packet.x + ((mouse_bytes[0] & MOUSE_X_SIGNAL) ? (0xFF00 | mouse_bytes[1]) : mouse_bytes[1]);
  int16_t delta_y = mouse_packet.y - ((mouse_bytes[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | mouse_bytes[2]) : mouse_bytes[2]);

  if (delta_x < 0 || delta_x > modeinf.XResolution || delta_y < 0 || delta_y > modeinf.YResolution)
    return;
  mouse_packet.x = delta_x;
  mouse_packet.y = delta_y;
}

int(mouse_write)(uint8_t command) {

   uint32_t mouse_response;

  do {
    if (write_KBC_command(KBC_IN_CMD, 0xD4))
      return 1;
    if (write_KBC_command(KBC_OUT_CMD, command))
      return 1;
    tickdelay(micros_to_ticks(WAIT_KBC));
    if (read_KBC_output(KBC_OUT_CMD, (uint8_t *)&mouse_response, 1) != 0)
      return 1;
    if (mouse_response == 0xFA)
      return 0;
  } while (mouse_response != 0xFA);

  return 1;
}

int set_sample_rate(uint8_t rate) {
  if (mouse_write(0xF3) != 0) { // Send "Set Sample Rate" command
    printf("mouse_write (Set Sample Rate) failed\n");
    return 1;
  }
  if (mouse_write(rate) != 0) { // Send the desired rate value
    printf("mouse_write (rate) failed\n");
    return 1;
  }
  return 0;
}
