/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/

#ifndef BMP_IMAGE_H
#define BMP_IMAGE_H
#include "struct.h"
#include <vector>
#include <iostream>

class BmpImage
{
private:
    BMPHeader header;
    DIBHeader dibHeader;
    int32_t width, height;
    std::vector<std::vector<Pixel>> data;
    std::vector<std::vector<double>> createGaussianKernel(int, double);
public:
    void load(const std::string&);
    void rotate90Clockwise();
    void rotate90CounterClockwise();
    void save(const std::string&);
    void gaussFilter(int, double);
};

#endif