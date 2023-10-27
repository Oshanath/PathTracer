#pragma once

#include "Util.h"

struct color
{
    double r;
    double g;
    double b;

    color& operator+=(const color& v) {
        r += v.r;
        g += v.g;
        b += v.b;
        return *this;
    }

    color& operator*=(double t) {
        r *= t;
        g *= t;
        b *= t;
        return *this;
    }

    color& operator/=(double t) {
        return *this *= 1 / t;
    }

    inline static color random()
    {
        return color(random_double(), random_double(), random_double());
    }

    inline static color random(double min, double max) {
        return color(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    friend color operator*(double t, const color& v);
    friend color operator*(const color& v, double t);
    friend color operator/(const color& v, double t);
    friend color operator*(const color& v1, const color& v2);
    friend color operator+(const color& u, const color& v);
};

inline color operator*(double t, const color& v) {
    return color(t * v.r, t * v.g, t * v.b);
}

inline color operator*(const color& v, double t) {
    return t * v;
}

inline color operator/(const color& v, double t) {
    return 1/t * v;
}

inline color operator+(const color& u, const color& v) {
    return color(u.r + v.r, u.g + v.g, u.b + v.b);
}

color operator*(const color& v1, const color& v2) {
    return color(v1.r * v2.r, v1.g * v2.g, v1.b * v2.b);
}