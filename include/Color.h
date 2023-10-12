#pragma once

#include "Util.h"

struct Color
{
    double r;
    double g;
    double b;

    Color& operator+=(const Color& v) {
        r += v.r;
        g += v.g;
        b += v.b;
        return *this;
    }

    Color& operator*=(double t) {
        r *= t;
        g *= t;
        b *= t;
        return *this;
    }

    Color& operator/=(double t) {
        return *this *= 1 / t;
    }

    inline static Color random()
    {
        return Color(random_double(), random_double(), random_double());
    }

    inline static Color random(double min, double max) {
        return Color(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    friend Color operator*(double t, const Color& v);
    friend Color operator*(const Color& v, double t);
    friend Color operator*(const Color& v1, const Color& v2);
    friend Color operator+(const Color& u, const Color& v);
};

inline Color operator*(double t, const Color& v) {
    return Color(t * v.r, t * v.g, t * v.b);
}

inline Color operator*(const Color& v, double t) {
    return t * v;
}

inline Color operator+(const Color& u, const Color& v) {
    return Color(u.r + v.r, u.g + v.g, u.b + v.b);
}

Color operator*(const Color& v1, const Color& v2) {
    return Color(v1.r * v2.r, v1.g * v2.g, v1.b * v2.b);
}