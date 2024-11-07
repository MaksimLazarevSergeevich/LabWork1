/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/

#include "bmp_image.h"

int main()
{
    const char* filename = "image.bmp";
    BmpImage img;
    img.loadFile(filename);
    img.rotateOclock();
    img.writeFile("rotated_90.bmp");
    img.rotateUnOclock();
    img.rotateUnOclock();
    img.writeFile("rotated_un90.bmp");
    img.gaussFilter(3, 5.0); //radius, sigma. If the program is running too long, you can change the values.
    img.writeFile("filter.bmp");
}