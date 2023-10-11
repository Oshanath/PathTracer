#include "Util.h"

double hit_sphere(const Point3& center, double radius, const Ray& r) {
    Vec3 oc = r.get_origin() - center;
    auto a = r.get_direction().length_squared();
    auto half_b = dot(oc, r.get_direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

int main() {

    HittableList world;

    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto material_left = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
    auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);

}