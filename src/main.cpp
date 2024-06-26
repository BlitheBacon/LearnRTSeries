#include "public/includes.h"

#include "public/bvh.h"
#include "public/camera.h"
#include "public/hittable.h"
#include "public/hittable_list.h"
#include "public/sphere.h"
#include "public/texture.h"

void bouncing_spheres()
{
    hittable_list world;

    // Ground sphere
    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            const auto choose_material = random_double();

            if (point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()); (center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> material_sphere;
                if (choose_material < 0.8)
                {
                    // Diffuse
                    auto albedo     = color::random() * color::random();
                    material_sphere = make_shared<lambertian>(albedo);
                    auto center2    = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, material_sphere));
                } else if (choose_material < 0.95)
                {
                    // Metal
                    auto albedo     = color::random(0.5, 1);
                    auto fuzz       = random_double(0.0, 0.5);
                    material_sphere = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, material_sphere));
                } else
                {
                    // Glass
                    material_sphere = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, material_sphere));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vFov     = 20;
    cam.lookFrom = point3(13, 2, 3);
    cam.lookAt   = point3(0, 0, 0);
    cam.vUp      = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);
}

void checkered_spheres()
{
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vFov     = 20;
    cam.lookFrom = point3(13, 2, 3);
    cam.lookAt   = point3(0, 0, 0);
    cam.vUp      = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void earth()
{
    auto earth_texture = make_shared<image_texture>("Images/earth.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe         = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vFov     = 20;
    cam.lookFrom = point3(0, 0, 12);
    cam.lookAt   = point3(0, 0, 0);
    cam.vUp      = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

int main()
{
    switch (3)
    {
        case 1: bouncing_spheres();
            break;
        case 2: checkered_spheres();
            break;
        case 3: earth();
            break;
    }
}
