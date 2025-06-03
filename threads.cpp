#include "threads.h"

void thread_1(BmpImage image)
{
    image.rotate90Clockwise();
    std::cout << "Rotated_90\n";
    image.save("rotated_90.bmp");
}

void thread_2(BmpImage image)
{
    image.rotate90CounterClockwise();
    std::cout << "Rotated_un_90\n";
    image.save("rotated_un90.bmp");

    image.gaussFilter(10, 5);
    std::cout << "Gaussed\n";
    image.save("filter.bmp");
}
