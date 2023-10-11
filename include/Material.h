#pragma once

#include "Image.h"
#include "Ray.h"

class HitRecord;

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, ColorRGB& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const ColorRGB& a) : albedo(a) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, ColorRGB& attenuation, Ray& scattered) const override {
        auto scatter_direction = rec.normal + Vec3::random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    ColorRGB albedo;
};

class Metal : public Material {
public:
    Metal(const ColorRGB& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, ColorRGB& attenuation, Ray& scattered)
        const override {
        Vec3 reflected = Vec3::reflect(unit_vector(r_in.get_direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * Vec3::random_unit_vector());
        attenuation = albedo;
        return (dot(scattered.get_direction(), rec.normal) > 0);
    }

private:
    ColorRGB albedo;
    double fuzz;
};
