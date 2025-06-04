#include <gtest/gtest.h>
#include "include/bmp_image.h"

TEST(BmpImageTest, LoadsValid24BitBmp)
{
    BmpImage image;
    ASSERT_NO_THROW(image.load("test_images/bmp_24bit_rgb.bmp"));

    EXPECT_EQ(image.getWidth(), 100);
    EXPECT_EQ(image.getHeight(), 100);

    auto p = image.getPixel();
    EXPECT_EQ(p[0][0].red, 255);
    EXPECT_EQ(p[0][0].green, 0);
    EXPECT_EQ(p[0][0].blue, 0);
}

TEST(BmpImageTest, ThrowsOnInvalidSignature)
{
    BmpImage image;
    EXPECT_THROW(image.load("test_images/not_a_bmp.txt"), std::runtime_error);
}

TEST(BmpImageTest, LoadsOneByOneBmp)
{
    BmpImage image;
    ASSERT_NO_THROW(image.load("test_images/bmp_1x1.bmp"));
    EXPECT_EQ(image.getWidth(), 1);
    EXPECT_EQ(image.getHeight(), 1);

    auto p = image.getPixel();
    EXPECT_EQ(p[0][0].red, 0);
    EXPECT_EQ(p[0][0].green, 0);
    EXPECT_EQ(p[0][0].blue, 255);
}

TEST(BmpImageTest, ThrowsOnCorruptedHeader)
{
    BmpImage image;
    EXPECT_THROW(image.load("test_images/bmp_corrupted_header.bmp"), std::runtime_error);
}

TEST(BmpImageTest, Rotate90ClockwiseOn5x10Image)
{
    BmpImage image;
    image.load("test_images/bmp_rectangle.bmp");

    int origWidth = image.getWidth();   // 5
    int origHeight = image.getHeight(); // 10
    auto before  = image.getPixel();

    image.rotate90Clockwise();
    auto after = image.getPixel();

    EXPECT_EQ(image.getWidth(), origHeight);  // 10
    EXPECT_EQ(image.getHeight(), origWidth);  // 5

    for (int y = 0; y < origHeight; ++y)
    {
        for (int x = 0; x < origWidth; ++x)
        {
            EXPECT_EQ(before[y][x].red, after[x][origHeight - 1 - y].red);
            EXPECT_EQ(before[y][x].blue, after[x][origHeight - 1 - y].blue);
            EXPECT_EQ(before[y][x].green, after[x][origHeight - 1 - y].green);
        }
    }
}

TEST(BmpImageTest, Rotate90CounterClockwiseOn5x10Image)
{
    BmpImage image;
    image.load("test_images/bmp_rectangle.bmp");

    int origWidth = image.getWidth();   // 5
    int origHeight = image.getHeight(); // 10
    auto before  = image.getPixel();

    image.rotate90CounterClockwise();
    auto after = image.getPixel();

    EXPECT_EQ(image.getWidth(), origHeight);  // 10
    EXPECT_EQ(image.getHeight(), origWidth);  // 5

    for (int y = 0; y < origHeight; ++y)
    {
        for (int x = 0; x < origWidth; ++x)
        {
            // (x, y) → (height - 1 - y, x)
            EXPECT_EQ(before[y][x].red,   after[origWidth - 1 - x][y].red);
            EXPECT_EQ(before[y][x].green, after[origWidth - 1 - x][y].green);
            EXPECT_EQ(before[y][x].blue,  after[origWidth - 1 - x][y].blue);
        }
    }
}


TEST(BmpImageTest, SaveFile)
{
    BmpImage image;
    image.load("test_images/bmp_rectangle.bmp");
    image.rotate90Clockwise();
    EXPECT_NO_THROW(image.save("test_images/rotated_rec_bmp.bmp"));
}

TEST(BmpImageTest, GaussBlurSpreadsWhiteCenter)
{
    BmpImage image;
    image.load("test_images/bmp_white_center.bmp");

    image.gaussFilter(1, 1.0);
    auto pixels = image.getPixel();
    // white center must become more gray
    Pixel center = pixels[2][2];
    EXPECT_LT(center.red, 255);

    // Pixels musnt be black
    EXPECT_GT(pixels[2][1].red, 0);
    EXPECT_GT(pixels[2][3].red, 0);
    EXPECT_GT(pixels[1][2].red, 0);
    EXPECT_GT(pixels[3][2].red, 0);
}
