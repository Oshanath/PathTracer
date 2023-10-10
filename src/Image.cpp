#include "Image.h"
#include <iostream>

Image::Image(int width, int height, std::string filename) : 
    image_width(width), 
    image_height(height),
    filename(filename),
    data(std::vector<std::vector<ColorRGB>>(image_height, std::vector<ColorRGB>(image_width))) {}

Image::~Image() {}

void Image::render() {

    std::ofstream file;
    file.open(filename, std::ios::out);

    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            file << data[j][i].r << ' ' << data[j][i].g << ' ' << data[j][i].b << '\n';
        }
    }

    file.close();
}

void Image::generate_test_gradient()
{
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0;

            data[j][i].r = static_cast<int>(255.999 * r);
            data[j][i].g = static_cast<int>(255.999 * g);
            data[j][i].b = static_cast<int>(255.999 * b);
        }
    }
}

void Image::write_color(int row, int col, ColorRGB pixel_color)
{
    data[row][col] = pixel_color;
}
