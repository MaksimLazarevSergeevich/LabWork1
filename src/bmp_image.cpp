/*Maksim Lazarev st128707@student.spbu.ru
first LabWork*/
#include <thread>
#include "include/bmp_image.h"
#include <fstream>
#include <cmath>

/**
 * @brief Loads a BMP image from a file into memory.
 * 
 * This function reads the BMP and DIB headers, checks validity,
 * loads pixel data, and stores it in the internal 2D vector.
 * 
 * @param filename Path to the BMP file.
 * @throw std::runtime_error If the file cannot be opened or is not a valid BMP.
 */
void BmpImage::load(const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in)
    {
        throw std::runtime_error("Unable to open BMP file.");
    }

    if (!in.read(reinterpret_cast<char*>(&header), sizeof(header)))
    {
        throw  std::runtime_error("Failed to read header");
    }

    if (!in.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader)))
    {
        throw std::runtime_error("Failed to read dibHeader");
    }

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

/**
 * @brief Rotates the image 90 degrees clockwise.
 * 
 * This operation swaps the image width and height, and rearranges
 * pixel data accordingly.
 */
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

/**
 * @brief Rotates the image 90 degrees counter-clockwise.
 * 
 * This operation swaps the image width and height, and rearranges
 * pixel data accordingly.
 */
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

/**
 * @brief Saves the current image to a BMP file.
 * 
 * The function writes BMP headers and pixel data to a file.
 * Pixels are written bottom-to-top with row padding as per BMP spec.
 * 
 * @param filename Path to the output BMP file.
 * @throw std::runtime_error If the output file cannot be created or opened.
 */
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

/**
 * @brief Generates a normalized 2D Gaussian kernel.
 * 
 * The kernel is used for Gaussian blur filtering. The values are computed
 * using the Gaussian function and then normalized so the sum of all
 * weights is 1.
 * 
 * @param radius Radius of the kernel (defines the size: 2*radius + 1).
 * @param sigma Standard deviation of the Gaussian distribution.
 * @return A 2D vector representing the normalized kernel.
 */
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
/**
 * @brief Applies a Gaussian blur to the image using a given radius and sigma.
 * 
 * Each pixel is updated by performing a convolution with the Gaussian kernel.
 * Edges are handled by skipping out-of-bounds coordinates.
 * 
 * @param radius Radius of the Gaussian kernel.
 * @param sigma Standard deviation (spread) of the Gaussian kernel.
 */
// void BmpImage::gaussFilter(int radius, double sigma)
// {
//     std::vector<std::vector<double>> kernel = createGaussianKernel(radius, sigma);

//     for (int y = 0; y < height; ++y)
//     {
//         for (int x = 0; x < width; ++x)
//         {
//             double sumR = 0.0, sumG = 0.0, sumB = 0.0;
//             for (int ky = -radius; ky <= radius; ++ky)
//             {
//                 for (int kx = -radius; kx <= radius; ++kx)
//                 {
//                     int ny = y + ky;
//                     int nx = x + kx;
//                     if (ny >= 0 && ny < height && nx >= 0 && nx < width)
//                     {
//                         sumR += data[ny][nx].red * kernel[ky + radius][kx + radius];
//                         sumG += data[ny][nx].green * kernel[ky + radius][kx + radius];
//                         sumB += data[ny][nx].blue * kernel[ky + radius][kx + radius];
//                     }
//                 }
//             }
//             data[y][x].red = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumR)));
//             data[y][x].green = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumG)));
//             data[y][x].blue = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumB)));
//         }
//     }
// }

/**
 * @brief Applies a Gaussian blur filter to the image using the given radius and sigma.
 * 
 * This method applies Gaussian convolution across the image using a multithreaded approach.
 * The image is divided into horizontal blocks (rows), and each thread processes a separate block.
 * The Gaussian kernel is generated once and shared across all threads for efficiency.
 * 
 * @param radius Radius of the Gaussian kernel.
 * @param sigma  Standard deviation for the Gaussian distribution.
 */
void BmpImage::gaussFilter(int radius, double sigma)
{
    // Generate the Gaussian kernel once
    std::vector<std::vector<double>> kernel = createGaussianKernel(radius, sigma);

    // Get the number of available hardware threads
    int num_threads = std::thread::hardware_concurrency();

    std::cout << "Found " << num_threads << " cores" << '\n';

    if (num_threads == 0)
    {
        std::cout << "Didn't find a core, 4 cores will be used\n";
        num_threads = 4;
    }

    std::vector<std::thread> threads;

    /**
     * @brief Lambda function to process a vertical slice of rows.
     * 
     * @param start_y Starting row index (inclusive).
     * @param end_y   Ending row index (exclusive).
     */
    auto worker = [this, &radius, &kernel](int start_y, int end_y)
    {
        for (int y = start_y; y < end_y; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                double sumR = 0.0, sumG = 0.0, sumB = 0.0;

                // Apply convolution using the kernel
                for (int ky = -radius; ky <= radius; ++ky)
                {
                    for (int kx = -radius; kx <= radius; ++kx)
                    {
                        int ny = y + ky;
                        int nx = x + kx;

                        // Only include pixels within image bounds
                        if (ny >= 0 && ny < height && nx >= 0 && nx < width)
                        {
                            sumR += data[ny][nx].red   * kernel[ky + radius][kx + radius];
                            sumG += data[ny][nx].green * kernel[ky + radius][kx + radius];
                            sumB += data[ny][nx].blue  * kernel[ky + radius][kx + radius];
                        }
                    }
                }

                // Clamp results to [0, 255] and write back to the image
                data[y][x].red   = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumR)));
                data[y][x].green = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumG)));
                data[y][x].blue  = static_cast<uint8_t>(std::min(255.0, std::max(0.0, sumB)));
            }
        }
    };

    // Compute the number of rows each thread should process
    int block = height / num_threads;

    // Launch threads with different row ranges
    for (int i = 0; i < num_threads; ++i)
    {
        int start = i * block;
        int end;

        // Let the last thread handle any remaining rows
        if (i == num_threads - 1)
        {
            end = height;
        }
        else
        {
            end = (i + 1) * block;
        }

        threads.push_back(std::thread(worker, start, end));
    }

    // Wait for all threads to complete
    for (auto& t : threads)
    {
        t.join();
    }
}

int32_t BmpImage::getWidth()
{
    return width;
}

int32_t BmpImage::getHeight()
{
    return height;
}

std::vector<std::vector<Pixel>> BmpImage::getPixel()
{
    return data;
}