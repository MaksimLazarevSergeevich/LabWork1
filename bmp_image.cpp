/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/

#include "bmp_image.h"
#include <fstream>
#include <iostream>
#include <cmath>

void BmpImage::loadFile(const char* filename)
{
    std::ifstream inputFile(filename, std::ios::binary);
    if(!inputFile)
    {
        std::cerr << "The file did not open" << '\n';
    }

    inputFile.read(reinterpret_cast<char*>(&_header), sizeof(_header));
    inputFile.read(reinterpret_cast<char*>(&_infoheader), sizeof(_infoheader));

    if (_header.file_type != 0x4D42)
    {
        std::cerr << "File is not BMP" << '\n';
    }

    _width = _infoheader.width;
    _height = _infoheader.height;
    _data.resize(_height, std::vector<Pixel>(_width));

    inputFile.seekg(_header.offset_data, std::ios::beg);

    for (int y = _height - 1; y >= 0; --y)
    {
        for (int x = 0; x < _width; ++x)
        {
            inputFile.read(reinterpret_cast<char*>(&_data[y][x]), sizeof(Pixel));
        }
    }
}

void BmpImage::writeFile(const char* filename)
{
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile)
    {
        std::cerr << "The file was not created" << '\n';
    }
    _header.file_size = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + _width * _height * sizeof(Pixel);

    _infoheader.width = _width;
    _infoheader.height = _height;

    outFile.write(reinterpret_cast<const char*>(&_header), sizeof(_header));
    outFile.write(reinterpret_cast<const char*>(&_infoheader), sizeof(_infoheader));

    for (int y = _height - 1; y >= 0; --y)
    {
        for (int x = 0; x < _width; ++x)
        {
            outFile.write(reinterpret_cast<const char*>(&_data[y][x]), sizeof(Pixel));
        }
    }
}

void BmpImage::rotateOclock()
{
    std::vector<std::vector<Pixel>> rotatedData(_width, std::vector<Pixel>(_height));

    for (int y = 0; y < _height; ++y)
    {
        for (int x = 0; x < _width; ++x)
        {
            rotatedData[x][_height - 1 - y] = _data[y][x];
        }
    }
    _data = rotatedData;
    std::swap(_width, _height);
}

void BmpImage::rotateUnOclock()
{
    std::vector<std::vector<Pixel>> rotatedData(_height, std::vector<Pixel>(_width));

    for (int y = 0; y < _height; ++y)
    {
        for (int x = 0; x < _width; ++x)
        {
            rotatedData[_width - 1 - x][y] = _data[y][x];
        }
    }
    _data = rotatedData;
    std::swap(_width, _height);
}

std::vector<std::vector<double>> BmpImage::createGaussianKernel(int radius, double sigma)
{
    int size = 2 * radius + 1;
    std::vector<std::vector<double>> kernel(size, std::vector<double>(size));
    double sum = 0.0;

    for (int i = -radius; i <= radius; ++i)
    {
        for (int j = -radius; j <= radius; ++j)
        {
            kernel[i + radius][j + radius] = std::exp(-(i * i + j * j) / (2 * sigma * sigma));
            sum += kernel[i + radius][j + radius];
        }
    }

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}

void BmpImage::gaussFilter(int radius, double sigma)
{
    std::vector<std::vector<double>> kernel = createGaussianKernel(radius, sigma);

    for (int y = 0; y < _height; ++y)
    {
        for (int x = 0; x < _width; ++x)
        {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for (int ky = -radius; ky <= radius; ++ky)
            {
                for (int kx = -radius; kx <= radius; ++kx)
                {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (ny >= 0 && ny < _height && nx >= 0 && nx < _width)
                    {
                        sumR += _data[ny][nx].red * kernel[ky + radius][kx + radius];
                        sumG += _data[ny][nx].green * kernel[ky + radius][kx + radius];
                        sumB += _data[ny][nx].blue * kernel[ky + radius][kx + radius];
                    }
                }
            }
            _data[y][x].red = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumR)));
            _data[y][x].green = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumG)));
            _data[y][x].blue = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumB)));
        }
    }
}

