#pragma once

#include "vec3.h"

class Ray {
public:
    Ray() {}

    Ray(const Point3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

    Point3 get_origin() const { return origin; }
    Vec3 get_direction() const { return direction; }

    Point3 at(double t) const {
        return origin + t * direction;
    }

private:
    Point3 origin;
    Vec3 direction;
};
