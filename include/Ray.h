#pragma once

#include "vec3.h"

class Ray {
public:
    Ray() {}

    Ray(const Point3& origin, const Vec3& direction) : origin(origin), direction(direction), tm(0)
    {}

    Ray(const Point3& origin, const Vec3& direction, double time = 0.0)
        : origin(origin), direction(direction), tm(time)
    {}

    Point3 get_origin() const { return origin; }
    Vec3 get_direction() const { return direction; }
    double time() const { return tm; }

    Point3 at(double t) const {
        return origin + t * direction;
    }

private:
    Point3 origin;
    Vec3 direction;
    double tm;
};
