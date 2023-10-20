#include "Util.h"

#include <chrono>

void random_spheres(hittable_list& world, Camera& cam) {
    
    auto checker = std::make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    auto ground_Material = std::make_shared<lambertian>(std::make_shared<solid_color>(color(0.5, 0.5, 0.5)));
    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_Material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> Sphere_Material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = std::make_shared<solid_color>(color::random() * color::random());
                    Sphere_Material = std::make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    world.add(std::make_shared<sphere>(center, 0.2, Sphere_Material));
                }
                else if (choose_mat < 0.95) {
                    // Metal
                    auto albedo = std::make_shared<solid_color>(color::random(0.5, 1));
                    auto fuzz = random_double(0, 0.5);
                    Sphere_Material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, Sphere_Material));
                }
                else {
                    // glass
                    Sphere_Material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, Sphere_Material));
                }
            }
        }
    }

    auto Material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, Material1));

    auto Material2 = std::make_shared<lambertian>(std::make_shared<solid_color>(color(0.4, 0.2, 0.1)));
    world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, Material2));

    auto Material3 = std::make_shared<Metal>(std::make_shared<solid_color>(color(0.7, 0.6, 0.5)), 0.0);
    world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, Material3));


    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;
}

void two_spheres(hittable_list& world, Camera& cam) {

    auto checker = std::make_shared<checker_texture>(0.8, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));


    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void earth(hittable_list& world, Camera& cam) {
    auto earth_texture = std::make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = std::make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);
    world.add(globe);

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(0, 0, 12);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void two_perlin_spheres(hittable_list& world, Camera& cam) {

    auto pertext = std::make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

int main() {
    
    hittable_list world;
    Camera cam;

    switch (4) {
        case 1: random_spheres(world, cam);         break;
        case 2: two_spheres(world, cam);            break;
        case 3:  earth(world, cam);                 break;
        case 4:  two_perlin_spheres(world, cam);    break;
    }

    world = hittable_list(std::make_shared<bvh_node>(world));
    auto start = std::chrono::high_resolution_clock::now();
    cam.render(world);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "rendering the scene took " << duration.count() << " seconds.\n";
}