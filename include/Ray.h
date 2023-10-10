#pragma once

#include "vec3.h"

class Ray {
public:
    Ray() {}

    Ray(const point3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

    point3 get_origin() const { return origin; }
    Vec3 get_direction() const { return direction; }

    point3 at(double t) const {
        return origin + t * direction;
    }

private:
    point3 origin;
    Vec3 direction;
};
