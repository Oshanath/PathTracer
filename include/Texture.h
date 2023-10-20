#pragma once

#include "Util.h"

class texture {
public:
    virtual ~texture() = default;

    virtual Color value(double u, double v, const Point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color(Color c) : color_value(c) {}

    solid_color(double red, double green, double blue) : solid_color(Color(red, green, blue)) {}

    Color value(double u, double v, const Point3& p) const override {
        return color_value;
    }

private:
    Color color_value;
};

class checker_texture : public texture {
public:
    checker_texture(double _scale, std::shared_ptr<texture> _even, std::shared_ptr<texture> _odd)
        : inv_scale(1.0 / _scale), even(_even), odd(_odd) {}

    checker_texture(double _scale, Color c1, Color c2)
        : inv_scale(1.0 / _scale),
        even(std::make_shared<solid_color>(c1)),
        odd(std::make_shared<solid_color>(c2))
    {}

    Color value(double u, double v, const Point3& p) const override {
        auto xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
        auto yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
        auto zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;
};
