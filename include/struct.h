/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/

#ifndef STRUCT_H
#define STRUCT_H
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader
{
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offsetData;
};

struct DIBHeader
{
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

struct Pixel
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

#endif