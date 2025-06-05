/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/

#ifndef BMP_IMAGE_H
#define BMP_IMAGE_H

#include "struct.h"
#include <vector>
#include <iostream>

/**
 * @class BmpImage
 * @brief Class for loading, manipulating, and saving BMP images.
 *
 */
class BmpImage
{
private:
    BMPHeader header;                        /**< BMP file header. */
    DIBHeader dibHeader;                     /**< DIB header with image metadata. */
    int32_t width, height;                   /**< Image dimensions (width x height). */
    std::vector<std::vector<Pixel>> data;    /**< 2D array of pixel data (BGR format). */

public:
    /**
     * @brief Loads a BMP image from file.
     * @param filename Path to the BMP file.
     * @throws std::runtime_error if the file is invalid or unreadable.
     */
    void load(const std::string& filename);

    /**
     * @brief Rotates the image 90 degrees clockwise.
     */
    void rotate90Clockwise();

    /**
     * @brief Rotates the image 90 degrees counterclockwise.
     */
    void rotate90CounterClockwise();

    /**
     * @brief Saves the image to a BMP file.
     * @param filename Path where the image will be saved.
     * @throws std::runtime_error if saving fails.
     */
    void save(const std::string& filename);

    /**
     * @brief Applies a Gaussian blur filter to the image.
     * @param radius Radius of the Gaussian kernel.
     * @param sigma Standard deviation of the Gaussian distribution.
     */
    void gaussFilter(int radius, double sigma);

    /**
     * @brief Creates a 2D Gaussian kernel matrix.
     * @param radius Radius of the kernel.
     * @param sigma Standard deviation.
     * @return 2D vector containing the Gaussian kernel.
     */
    std::vector<std::vector<double>> createGaussianKernel(int radius, double sigma);

    /**
     * @brief Returns the width of the image.
     * @return Image width in pixels.
     */
    int32_t getWidth();

    /**
     * @brief Returns the height of the image.
     * @return Image height in pixels.
     */
    int32_t getHeight();

    /**
     * @brief Returns the full 2D pixel matrix.
     * @return 2D vector of pixels (BGR).
     */
    std::vector<std::vector<Pixel>> getPixel();
};

#endif
