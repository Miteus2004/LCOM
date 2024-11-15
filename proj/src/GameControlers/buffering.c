#include "buffering.h"
char *main_frame_buffer;
char *secondary_frame_buffer;
extern char *vidio_mem;
uint32_t frame_buffer_size;
extern int timer_interrupts;
extern vbe_mode_info_t modeinf;
extern MouseInfo mouse_packet;
extern bool Otimized;
extern int fps;
/**
 * The buffering module is responsible for handling frame buffers used in rendering graphics for 
 * the game. It provides functionality to set up and manage the main and secondary frame buffers, 
 * allowing for optimized rendering techniques such as double buffering. The module includes 
 * functions to initialize the frame buffers based on the current video mode and to swap buffers 
 * efficiently. This swapping mechanism is crucial for minimizing screen tearing and ensuring 
 * smooth visual updates.
 */
int(set_frame_buffers)(uint16_t mode) {

  if (Otimized) {

    if (frame_buffer_with_buff(mode, main_frame_buffer)) {
      printf("frame_buffer_with_buff failed\n");
      return 1;
    }

    vidio_mem = secondary_frame_buffer;
  }
  else {
    if (frame_buffer(mode)) {
      printf("frame_buffer failed\n");
      return 1;
    }
  }
  return 0;
}

void(swap_buffers)() {
  memcpy(main_frame_buffer, secondary_frame_buffer, frame_buffer_size);
}
