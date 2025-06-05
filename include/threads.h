#ifndef THREADS_H
#define THREADS_H
#include <thread>
#include "bmp_image.h"

/**
 * @brief Thread function that rotates the image
 *
 * @param image A copy of the BmpImage to be processed.
 */
void thread_1(BmpImage);
/**
 * @brief This function rotates the image and calls the Gaussian filter
 * 
 * @param image A copy of the BmpImage to be processed.
 */
void thread_2(BmpImage);

#endif