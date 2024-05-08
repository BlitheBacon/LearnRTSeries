#include "includes.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main()
{
	hittable_list world;

	// Ground spehere
	auto material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, material_ground));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			auto choose_material = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9)
			{
				shared_ptr<material> material_sphere;
				if (choose_material < 0.8)
				{
					// Diffuse
					auto albedo = color::random() * color::random();
					material_sphere = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, material_sphere));
				}
				else if (choose_material < 0.95)
				{
					// Metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0.0, 0.5);
					material_sphere = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, material_sphere));
				}
				else
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

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1200;
	cam.samples_per_pixel = 500;
	cam.max_depth = 50;

	cam.vFov = 20;
	cam.lookFrom = point3(13, 2, 3);
	cam.lookAt = point3(0, 0, 0);
	cam.vUp = vec3(0, 1, 0);

	cam.defocus_angle = 0.6;
	cam.focus_dist = 10.0;

	cam.render(world);
}
