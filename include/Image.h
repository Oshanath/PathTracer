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

class Image
{

public:
    const int image_width;
    const int image_height;
    const std::string filename;

    Image(int width, int height, std::string filename);
    ~Image();

    void render();
    void generate_test_gradient();
    void write_color(int row, int col, ColorRGB pixel_color);

private:
    std::vector<std::vector<ColorRGB>> data;
};