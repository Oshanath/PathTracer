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

ColorRGB ray_color(const Ray& r, const Hittable& world) {

    auto t = hit_sphere(Point3(0, 0, -1), 0.5, r);

    HitRecord rec;
    if (world.hit(r, Interval(0, infinity), rec)) {
        return 0.5 * (ColorRGB(rec.normal.x(), rec.normal.y(), rec.normal.z()) + ColorRGB(1, 1, 1));
    }

    Vec3 unit_direction = unit_vector(r.get_direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * ColorRGB(1.0, 1.0, 1.0) + a * ColorRGB(0.5, 0.7, 1.0);
}

int main() {

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    Image image(image_width, image_height, "image.ppm");
    std::cout << "width = " << image_width << ", height = " << image_height << '\n';

    // World

    HittableList world;
    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    auto camera_center = Point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = Vec3(viewport_width, 0, 0);
    auto viewport_v = Vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
        - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    for (int j = 0; j < image_height; ++j) {
        std::cout << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            Ray r(camera_center, ray_direction);

            ColorRGB pixel_color = ray_color(r, world);
            image.write_color(j, i, pixel_color);
        }
    }

    std::cout << "\rDone.                 \n";

    image.render();

}