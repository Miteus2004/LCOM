#ifndef BUFFERING_H
#define BUFFERING_H

#include <lcom/lcf.h>
#include "aimTrainControler.h"
// Add any necessary includes here

/**
 * @brief Initializes the frame buffers based on the provided video mode.
 * This function sets up the frame buffers for rendering graphics. If optimization
 * is enabled, it utilizes double buffering by allocating memory for both the main
 * and secondary frame buffers. Otherwise, it initializes a single frame buffer.
 * @param mode The video mode used for initialization.
 * @return 0 on success, 1 on failure.
 */
int (set_frame_buffers)(uint16_t mode);

/**
 * @brief Swaps the contents of the secondary frame buffer to the main frame buffer.
 * This function performs the buffer swap necessary for optimized rendering. It copies
 * the contents of the off-screen rendered frame from the secondary buffer to the main
 * buffer, ensuring smooth visual updates.
 */
void (swap_buffers)();

// Implement the member functions here

#endif // BUFFERING_H
