#pragma once

#include "vec3.h"

class ray {
public:
    ray() {}

    ray(const point3& origin, const vec3& direction) : origin(origin), direction(direction), tm(0)
    {}

    ray(const point3& origin, const vec3& direction, double time = 0.0)
        : origin(origin), direction(direction), tm(time)
    {}

    point3 get_origin() const { return origin; }
    vec3 get_direction() const { return direction; }
    double time() const { return tm; }

    point3 at(double t) const {
        return origin + t * direction;
    }

private:
    point3 origin;
    vec3 direction;
    double tm;
};
