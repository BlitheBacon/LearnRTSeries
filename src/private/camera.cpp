#include "camera.h"


void camera::initialize()
{
    // Calculate the image height, and ensure that it's at least 1.
    image_height = ((static_cast<int>(image_width / aspect_ratio)) < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = lookFrom;

    // Determine viewport dimensions.
    const auto theta           = degrees_to_radians(vFov);
    const auto h               = tan(theta / 2);
    const auto viewport_height = 2 * h * focus_dist;
    const auto viewport_width  = viewport_height * (static_cast<double>(image_width) / image_height);

    // Calculate the u, v, w unit basis vectors for the camera coordinate frame.
    w = unit_vector(lookFrom - lookAt);
    u = unit_vector(cross(vUp, w));
    v = cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const auto viewport_u = viewport_width * u;   // Vector across viewport horizontal edge
    const auto viewport_v = viewport_height * -v; // Vector down viewport vertical edge

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    const auto viewport_upper_left = center - (focus_dist * w) - (viewport_u / 2) - (viewport_v / 2);
    pixel100_loc                   = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Calculate the camera defocus disk basis vectors.
    const auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u            = u * defocus_radius;
    defocus_disk_v            = v * defocus_radius;
}


void camera::render(const hittable &world)
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

vec3 camera::sample_square() { return {random_double() - 0.5, random_double() - 0.5, 0}; }

point3 camera::defocus_disk_sample() const
{
    // Returns a random point in the camera defocus disk.
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

ray camera::get_ray(const int i, const int j) const
{
    // Construct a camera ray originating from the defocus disk and directed at a randomly
    // sampled point around the pixel location i, j.

    const auto offset       = sample_square();
    const auto pixel_sample = pixel100_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

    const auto ray_origin    = (defocus_angle <= 0) ? center : defocus_disk_sample();
    const auto ray_direction = pixel_sample - ray_origin;
    const auto ray_time      = random_double();

    return {ray_origin, ray_direction, ray_time};
}

color camera::ray_color(const ray &r, const int depth, const hittable &world) const
{
    // If we exceed the ray bounce limit, no more light is gathered.
    if (depth <= 0) return {0, 0, 0};

    if (hit_record rec; world.hit(r, interval(0.001, infinity), rec))
    {
        ray   scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) return attenuation * ray_color(scattered, depth - 1, world);
        return {0, 0, 0};
    }

    const vec3 unit_direction = unit_vector(r.direction());
    const auto a              = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
