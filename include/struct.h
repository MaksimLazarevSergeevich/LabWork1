/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/

#ifndef STRUCT_H
#define STRUCT_H

#include <cstdint>

/**
 * @brief BMP file header.
 *
 * Contains the file signature, file size, reserved fields,
 * and the offset to the start of pixel data.
 */
#pragma pack(push, 1)
struct BMPHeader
{
    uint16_t fileType;     /**< File type signature. Must be 0x4D42 ("BM"). */
    uint32_t fileSize;     /**< Total size of the BMP file in bytes. */
    uint16_t reserved1;    /**< Reserved1 */
    uint16_t reserved2;    /**< Reserved2 */
    uint32_t offsetData;   /**< Offset in bytes to the start of the pixel data. */
};

/**
 * @brief DIB header.
 *
 * Describes the dimensions, color format, compression, and resolution of the bitmap image.
 */
struct DIBHeader
{
    uint32_t size;             /**< Size of this header. */
    int32_t width;             /**< Image width in pixels. */
    int32_t height;            /**< Image height in pixels. */
    uint16_t planes;           /**< Number of color planes. */
    uint16_t bitCount;         /**< Bits per pixel. */
    uint32_t compression;      /**< Compression method. */
    uint32_t imageSize;        /**< Size of the raw image data. */
    int32_t xPixelsPerMeter;   /**< Horizontal resolution. */
    int32_t yPixelsPerMeter;   /**< Vertical resolution. */
    uint32_t colorsUsed;       /**< Number of colors used in the palette. */
    uint32_t colorsImportant;  /**< Number of important colors. */
};
#pragma pack(pop)

/**
 * @brief A single pixel in BGR format.
 *
 * Represents the color of a pixel using blue, green, and red channels.
 */
struct Pixel
{
    uint8_t blue;   /**< Blue color component. */
    uint8_t green;  /**< Green color component. */
    uint8_t red;    /**< Red color component. */
};

#endif
