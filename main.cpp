/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/

#include "bmp_image.h"

int main()
{
    BmpImage image;
    image.load("ima.bmp");
    image.rotate90Clockwise();
    std::cout << "Rotated" << '\n';
    image.save("rotated_90.bmp");
    std::cout << "Saved rotated_90.bmp" << '\n';
    image.rotate90CounterClockwise();
    std::cout << "Rotated" << '\n';
    image.rotate90CounterClockwise();
    std::cout << "Rotated" << '\n';
    image.save("rotated_un90.bmp");
    std::cout << "Saved rotated_un90.bmp" << '\n';
    image.gaussFilter(3, 5); //radius, sigma. If the program is running too long, you can change the values.
    image.save("filter.bmp");
    std::cout << "Saved filter.bmp" << '\n';
    return 0;
}