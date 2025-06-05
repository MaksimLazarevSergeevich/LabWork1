/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/
#include <chrono>
#include "include/threads.h"

// int main()
// {
//     auto start = std::chrono::high_resolution_clock::now();
//     BmpImage image;
//     image.load("ima.bmp");
//     image.rotate90Clockwise();
//     std::cout << "Rotated" << '\n';
//     image.save("rotated_90.bmp");
//     std::cout << "Saved rotated_90.bmp" << '\n';
//     image.rotate90CounterClockwise();
//     std::cout << "Rotated" << '\n';
//     image.rotate90CounterClockwise();
//     std::cout << "Rotated" << '\n';
//     image.save("rotated_un90.bmp");
//     std::cout << "Saved rotated_un90.bmp" << '\n';
//     image.gaussFilter(10, 5); //radius, sigma. If the program is running too long, you can change the values.
//     image.save("filter.bmp");
//     std::cout << "Saved filter.bmp" << '\n';
    

//     auto end = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> duration = end - start;

//    std::cout << "Время выполнения: " << duration.count() << " секунд\n";
//     return 0;
// }

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    BmpImage image;
    image.load("test_images/ima.bmp");

    std::thread th_1(thread_1, image);
    std::thread th_2(thread_2, image);

    th_1.join();
    th_2.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Execution time: " << duration.count() << " seconds\n";
    return 0;
}
