/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/

#include "bmp_image.h"
#include <fstream>
#include <iostream>
#include <cmath>

void BmpImage::load(const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in)
    {
        throw std::runtime_error("Unable to open BMP file.");
    }

    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    in.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));

    if (header.fileType != 0x4D42)
    {
        throw std::runtime_error("Not a valid BMP file.");
    }

    width = dibHeader.width;
    height = dibHeader.height;

    int rowSize = (width * sizeof(Pixel) + 3) & ~3;
    int paddingSize = rowSize - width * sizeof(Pixel);

    data.resize(height, std::vector<Pixel>(width));

    in.seekg(header.offsetData, std::ios::beg);

    for (int y = height - 1; y >= 0; --y)
    {
        for (int x = 0; x < width; ++x)
        {
            in.read(reinterpret_cast<char*>(&data[y][x]), sizeof(Pixel));
        }
        in.ignore(paddingSize);
    }
}


void BmpImage::rotate90Clockwise()
{
    std::vector<std::vector<Pixel>> rotatedData(width, std::vector<Pixel>(height));

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            rotatedData[x][height - 1 - y] = data[y][x];
        }
    }
    data = rotatedData;
    std::swap(width, height);
}

void BmpImage::rotate90CounterClockwise()
{
    std::vector<std::vector<Pixel>> rotatedData(width, std::vector<Pixel>(height));

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            rotatedData[width - 1 - x][y] = data[y][x];
        }
    }
    data = rotatedData;
    std::swap(width, height);
}

void BmpImage::save(const std::string& filename)
{
    std::ofstream out(filename, std::ios::binary);
    if (!out)
    {
        throw std::runtime_error("Unable to open output file.");
    }

    int rowSize = (width * sizeof(Pixel) + 3) & ~3;
    int paddingSize = rowSize - width * sizeof(Pixel);

    header.fileSize = sizeof(BMPHeader) + sizeof(DIBHeader) + rowSize * height;
    dibHeader.width = width;
    dibHeader.height = height;

    out.write(reinterpret_cast<const char*>(&header), sizeof(header));
    out.write(reinterpret_cast<const char*>(&dibHeader), sizeof(dibHeader));

    for (int y = height - 1; y >= 0; --y)
    {
        for (int x = 0; x < width; ++x)
        {
            out.write(reinterpret_cast<const char*>(&data[y][x]), sizeof(Pixel));
        }
        for (int p = 0; p < paddingSize; ++p)
        {
            out.put(0);
        }
    }
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

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for (int ky = -radius; ky <= radius; ++ky)
            {
                for (int kx = -radius; kx <= radius; ++kx)
                {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (ny >= 0 && ny < height && nx >= 0 && nx < width)
                    {
                        sumR += data[ny][nx].red * kernel[ky + radius][kx + radius];
                        sumG += data[ny][nx].green * kernel[ky + radius][kx + radius];
                        sumB += data[ny][nx].blue * kernel[ky + radius][kx + radius];
                    }
                }
            }
            data[y][x].red = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumR)));
            data[y][x].green = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumG)));
            data[y][x].blue = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumB)));
        }
    }
}