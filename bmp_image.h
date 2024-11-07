/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/

#ifndef BMP_IMAGE_H
#define BMP_IMAGE_H
#include "struct.h"
#include <vector>

class BmpImage
{
private:
    BMPHeader _header;
    BMPInfoHeader _infoheader;
    uint32_t _height;
    uint32_t _width;
    std::vector<std::vector<Pixel>> _data;
    std::vector<std::vector<double>> createGaussianKernel(int, double);
public:
    void loadFile(const char*);
    void writeFile(const char*);
    void rotateOclock();
    void rotateUnOclock();
    void gaussFilter(int, double);
};

#endif