#pragma once

#include <fstream>
#include <vector>

struct ColorRGB
{
    double r;
    double g;
    double b;

    ColorRGB& operator+=(const ColorRGB& v) {
        r += v.r;
        g += v.g;
        b += v.b;
        return *this;
    }

    ColorRGB& operator*=(double t) {
        r *= t;
        g *= t;
        b *= t;
        return *this;
    }

    ColorRGB& operator/=(double t) {
        return *this *= 1 / t;
    }

    friend ColorRGB operator*(double t, const ColorRGB& v);
    friend ColorRGB operator*(const ColorRGB& v, double t);
    friend ColorRGB operator*(const ColorRGB& v1, const ColorRGB& v2);
    friend ColorRGB operator+(const ColorRGB& u, const ColorRGB& v);
};

inline ColorRGB operator*(double t, const ColorRGB& v) {
    return ColorRGB(t * v.r, t * v.g, t * v.b);
}

inline ColorRGB operator*(const ColorRGB& v, double t) {
    return t * v;
}

inline ColorRGB operator+(const ColorRGB& u, const ColorRGB& v) {
    return ColorRGB(u.r + v.r, u.g + v.g, u.b + v.b);
}

ColorRGB operator*(const ColorRGB& v1, const ColorRGB& v2) {
    return ColorRGB(v1.r * v2.r, v1.g * v2.g, v1.b * v2.b);
}


inline static double linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

class Image
{

public:
    const int image_width;
    const int image_height;
    const std::string filename;

    inline Image(int width, int height, std::string filename) :
        image_width(width),
        image_height(height),
        filename(filename),
        data(std::vector<std::vector<ColorRGB>>(image_height, std::vector<ColorRGB>(image_width))) {}

    inline ~Image() {}

    inline void render() {

        std::ofstream file;
        file.open(filename, std::ios::out);

        file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                file << int(255 * data[j][i].r) << ' ' << int(255 * data[j][i].g) << ' ' << int(255 * data[j][i].b) << '\n';
            }
        }

        file.close();
    }

    inline void generate_test_gradient()
    {
        for (int j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                auto r = double(i) / (image_width - 1);
                auto g = double(j) / (image_height - 1);
                auto b = 0;

                data[j][i].r = static_cast<int>(r);
                data[j][i].g = static_cast<int>(g);
                data[j][i].b = static_cast<int>(b);
            }
        }
    }

    inline void write_color(int row, int col, ColorRGB pixel_color)
    {
        data[row][col] = pixel_color;
    }

private:
    std::vector<std::vector<ColorRGB>> data;
};