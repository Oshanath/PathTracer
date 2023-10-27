#pragma once

#include "Image.h"
#include "Hittable.h"
#include "Material.h"
#include "pdf.h"

class Camera {

public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width = 100;  // Rendered image width in pixel count
    int    samples_per_pixel = 10;   // Count of random samples for each pixel
    int    max_depth = 10;   // Maximum number of ray bounces into scene
    color  background;               // Scene background color
    double vfov = 90;  // Vertical view angle (field of view)
    point3 lookfrom = point3(0, 0, -1);  // Point camera is looking from
    point3 lookat = point3(0, 0, 0);   // Point camera is looking at
    vec3   vup = vec3(0, 1, 0);     // Camera-relative "up" direction

    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus

    void render(const hittable& world, const hittable& lights) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        int counter = 0;

        #pragma omp parallel for
        for (int j = 0; j < image_height; ++j) {

            #pragma omp parallel for
            for (int i = 0; i < image_width; ++i) {

                const interval intensity(0.0, 0.999);
                color pixel_color(0, 0, 0);

                #pragma omp parallel for
                for (int s_j = 0; s_j < int(sqrt_spp); ++s_j) {

                    #pragma omp parallel for
                    for (int s_i = 0; s_i < int(sqrt_spp); ++s_i) {
                        ray r = get_ray(i, j, s_i, s_j);
                        color c = ray_color(r, max_depth, world, lights);
                        pixel_color += (c.r != c.r || c.g != c.g or c.b != c.b) ? color(0.0, 0.0, 0.0) : ray_color(r, max_depth, world, lights);
                    }
                }

                pixel_color /= samples_per_pixel;

                // Apply the linear to gamma transform.
                pixel_color = color(
                    linear_to_gamma(pixel_color.r),
                    linear_to_gamma(pixel_color.g),
                    linear_to_gamma(pixel_color.b)
                );

                image_ptr->write_color(j, i, pixel_color);

                #pragma omp critical
                {
                    counter++;

                    if (counter % 1000 == 0)
                        std::cout << image_width * image_height - counter << " pixels remaining." << std::endl;
                }
            }

        }
        

        image_ptr->render();

        std::clog << "\rDone.                 \n";
    }

private:
    int    image_height;   // Rendered image height
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    vec3   u, v, w;        // Camera frame basis vectors
    vec3   defocus_disk_u;  // Defocus disk horizontal radius
    vec3   defocus_disk_v;  // Defocus disk vertical radius
    float sqrt_spp;		// Square root of samples per pixel
    float recip_sqrt_spp;		// Square root of samples per pixel
    std::unique_ptr<Image> image_ptr;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        sqrt_spp = sqrt(samples_per_pixel);
        recip_sqrt_spp = 1.0 / sqrt_spp;

        center = lookfrom;

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;

        image_ptr = std::make_unique<Image>(image_width, image_height, "image.ppm");
    }

    color ray_color(const ray& r, int depth, const hittable& world, const hittable& lights) const {

        hit_record rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0, 0, 0);

        // If the ray hits nothing, return the background color.
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        double pdf_val;
        color color_from_emission = rec.mat->emitted(r, rec, rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered, pdf_val))
            return color_from_emission;

        hittable_list* light_ptr = (hittable_list*)&lights;

        auto p0 = std::make_shared<hittable_pdf>(*(light_ptr->objects[0]), rec.p);
        auto p1 = std::make_shared<cosine_pdf>(rec.normal);
        mixture_pdf mixed_pdf(p0, p1);

        scattered = ray(rec.p, mixed_pdf.generate(), r.time());
        pdf_val = mixed_pdf.value(scattered.get_direction());

        double scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);

        color sample_color = ray_color(scattered, depth - 1, world, lights);
        color color_from_scatter = (attenuation * scattering_pdf * sample_color) / pdf_val;

        return color_from_emission + color_from_scatter;
    }

    ray get_ray(int i, int j, int s_i, int s_j) const {
        // Get a randomly-sampled camera ray for the pixel at location i,j, originating from
        // the camera defocus disk, and randomly sampled around the pixel location.

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square(s_i, s_j);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = vec3::random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    vec3 pixel_sample_square(int s_i, int s_j) const {
        // Returns a random point in the square surrounding a pixel at the origin, given
        // the two subpixel indices.
        auto px = -0.5 + recip_sqrt_spp * (s_i + random_double());
        auto py = -0.5 + recip_sqrt_spp * (s_j + random_double());
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
};
