#pragma once

#include "ray.h"
#include "Interval.h"
#include "aabb.h"

class Material;

class HitRecord {
  public:
    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> mat;
    double t;
    double u;
    double v;
    bool front_face;

    void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.get_direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
  public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;

    virtual aabb bounding_box() const = 0;
};