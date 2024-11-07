#include "bmpimage.h"
#include <fstream>
#include <iostream>

void BmpImage::loadFile(const char* filename) {
    std::ifstream inputFile(filename, std::ios::binary);
    if(!inputFile) {
        std::cerr << "Cannot open the file" << '\n';
    }

    inputFile.read(reinterpret_cast<char*>(&_header), sizeof(_header));
    inputFile.read(reinterpret_cast<char*>(&_infoheader), sizeof(_infoheader));

    if (_header.file_type != 0x4D42) {
        std::cerr << "File is not BMP" << '\n';
    }

    _width = _infoheader.width;
    _height = _infoheader.height;
    _data.resize(_height, std::vector<Pixel>(_width));

    inputFile.seekg(_header.offset_data, std::ios::beg);

    for (int y = _height - 1; y >= 0; --y) {
        for (int x = 0; x < _width; ++x) {
            inputFile.read(reinterpret_cast<char*>(&_data[y][x]), sizeof(Pixel));
        }
    }
}

void BmpImage::writeFile(const char* filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Cannot create outfile" << '\n';
    }
    _header.file_size = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + _width * _height * sizeof(Pixel);
    
    _infoheader.width = _width;
    _infoheader.height = _height;

    outFile.write(reinterpret_cast<const char*>(&_header), sizeof(_header));
    outFile.write(reinterpret_cast<const char*>(&_infoheader), sizeof(_infoheader));

    for (int y = _height - 1; y >= 0; --y) {
        for (int x = 0; x < _width; ++x) {
            outFile.write(reinterpret_cast<const char*>(&_data[y][x]), sizeof(Pixel));
        }
    }
}

void BmpImage::rotateOclock() {
    std::vector<std::vector<Pixel>> rotatedData(_width, std::vector<Pixel>(_height));

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
                rotatedData[x][_height - 1 - y] = _data[y][x];
            }
        }
    _data = rotatedData;
    std::swap(_width, _height);
    }

void BmpImage::rotateUnOclock() {
    std::vector<std::vector<Pixel>> rotatedData(_height, std::vector<Pixel>(_width));

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            rotatedData[_width - 1 - x][y] = _data[y][x];
        }
    }
    _data = rotatedData;
    std::swap(_width, _height);
}


