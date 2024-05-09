#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera
{
 public:
	double aspect_ratio = 1.0; // Ratio of image width over height
	int image_width = 100; // Rendered image width in pixel count
	int samples_per_pixel = 10; // Count of random samples for each pixel
	int max_depth = 10; // Maximum number of ray bounces into scene

	double vFov = 90; // Vertical view angle (Field of View)
	point3 lookFrom = point3(0, 0, 0); // Point camera is looking from
	point3 lookAt = point3(0, 0, -1); // Point camera is looking at
	vec3 vUp = point3(0, 1, 0); // Camera relative "up" direction

	double defocus_angle = 0; // Variation angle of rays through each pixel
	double focus_dist = 10; // Distance from camera lookFrom point to plane of perfect focus

	void render(const hittable& world)
	{
		initialize();

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; j++)
		{
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; i++)
			{
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++)
				{
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(std::cout, pixel_samples_scale * pixel_color);
			}
		}
		std::clog << "\rDone.               \n";
	}

 private:
	int image_height; // Rendered image height
	double pixel_samples_scale; // Color scale factor for a sum of pixel samples
	point3 center; // Camera center
	point3 pixel100_loc; // Location of pixel 0, 0
	vec3 pixel_delta_u; // Offset to pixel to the right
	vec3 pixel_delta_v; // Offset to pixel below
	vec3 u, v, w; // Camera frame basis vectors
	vec3 defocus_disk_u; // Defocus disk horizontal radius
	vec3 defocus_disk_v; // Defocus disk vertical radius

	void initialize()
	{
		// Calculate the image height, and ensure that it's at least 1.
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.0 / samples_per_pixel;

		center = lookFrom;

		// Determine viewport dimensions.
		auto theta = degrees_to_radians(vFov);
		auto h = tan(theta / 2);
		auto viewport_height = 2 * h * focus_dist;
		auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

		// Calculate the u, v, w unit basis vectors for the camera coordinate frame.
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(vUp, w));
		v = cross(w, u);

		// Calculate the vectors across the horizontal and down the vertical viewport edges.
		auto viewport_u = viewport_width * u; // Vector across viewport horizontal edge
		auto viewport_v = viewport_height * -v; // Vector down viewport vertical edge

		// Calculate the horizontal and vertical delta vectors from pixel to pixel.
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate the location of the upper left pixel
		auto viewport_upper_left = center - (focus_dist * w) - (viewport_u / 2) - (viewport_v / 2);
		pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		// Calculate the camera defocus disk basis vectors.
		auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	ray get_ray(const int i, const int j) const
	{
		// Construct a camera ray originating from the defocus disk and directed at a randomly
		// sampled point around the pixel location i, j.

		const auto offset = sample_square();
		const auto pixel_sample = pixel100_loc
							+ ((i + offset.x()) * pixel_delta_u)
							+ ((j + offset.y()) * pixel_delta_v);

		const auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
		const auto ray_direction = pixel_sample - ray_origin;
		const auto ray_time = random_double();

		return {ray_origin, ray_direction, ray_time};
	}

	static vec3 sample_square()
	{
		// Returns the vector to a random point in the [-.5, -.5] - [+.5, +.5] unit square.
		return { random_double() - 0.5, random_double() - 0.5, 0 };
	}

	point3 defocus_disk_sample() const
	{
		// Returns a random point in the camera defocus disk.
		auto p = random_in_unit_disk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}

	color ray_color(const ray& r, int depth, const hittable& world) const
	{
		// If we exceed the ray bounce limit, no more light is gathered.
		if (depth <= 0)
			return { 0, 0, 0 };

		if (hit_record rec;
			world.hit(r, interval(0.001, infinity), rec))
		{
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_color(scattered, depth - 1, world);
			return { 0, 0, 0 };
		}

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif
