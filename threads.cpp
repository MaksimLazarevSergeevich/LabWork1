#include "threads.h"

/**
 * @brief Rotates the image 90 degrees clockwise and saves the result.
 * 
 * This function is intended to be run in a separate thread. It performs a 90-degree
 * clockwise rotation of the input image and saves the result to "rotated_90.bmp".
 * 
 * @param image A copy of the BmpImage object to be rotated and saved.
 */
void thread_1(BmpImage image)
{
    image.rotate90Clockwise();
    std::cout << "Rotated_90\n";
    image.save("rotated_90.bmp");
}

/**
 * @brief Rotates the image 90 degrees counter-clockwise, applies a Gaussian filter, and saves the results.
 * 
 * This function is intended to be run in a separate thread. It performs a 90-degree counter-clockwise
 * rotation of the image, applies a Gaussian blur filter with a given radius and sigma, and saves
 * two intermediate results: one after rotation and one after filtering.
 * 
 * @param image A copy of the BmpImage object to be transformed and saved.
 *              The function does not modify the original image outside the thread.
 */
void thread_2(BmpImage image)
{
    image.rotate90CounterClockwise();
    std::cout << "Rotated_un_90\n";
    image.save("rotated_un90.bmp");

    image.gaussFilter(10, 5);
    std::cout << "Gaussed\n";
    image.save("filter.bmp");
}

