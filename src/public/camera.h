#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera
{
public:
    /// Initialize Camera
    /// @details This method calculates and assigns various viewport parameters.
    void initialize();

    /// Render Image
    /// @details This method calls camera::initialize, then iterates through each pixel within the viewport, additively sampling color at the given ray
    /// location, per number of samples. The final pixel color is written to file.
    void render(const hittable &world);

    /// Sample Unit Square
    /// @return A 3-dimensional vector of a random point in the [-0.5, -0.5] -> [+0.5, +0.5] unit square, such that X and Y are random values and Z is 0.
    static vec3 sample_square();

    point3 defocus_disk_sample() const;

    ray get_ray(const int i, const int j) const;

    color ray_color(const ray &r, const int depth, const hittable &world) const;

public:
    double aspect_ratio      = 1.0; // Ratio of image width over height
    int    image_width       = 100; // Rendered image width in pixel count
    int    samples_per_pixel = 10;  // Count of random samples for each pixel
    int    max_depth         = 10;  // Maximum number of ray bounces into scene

    double vFov     = 90;               // Vertical view angle (Field of View)
    point3 lookFrom = point3(0, 0, 0);  // Point camera is looking from
    point3 lookAt   = point3(0, 0, -1); // Point camera is looking at
    vec3   vUp      = point3(0, 1, 0);  // Camera relative "up" direction

    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist    = 10; // Distance from camera lookFrom point to plane of perfect focus

private:
    int    image_height{};        // Rendered image height
    double pixel_samples_scale{}; // Color scale factor for a sum of pixel samples
    point3 center;                // Camera center
    point3 pixel100_loc;          // Location of pixel 0, 0
    vec3   pixel_delta_u;         // Offset to pixel to the right
    vec3   pixel_delta_v;         // Offset to pixel below
    vec3   u, v, w;               // Camera frame basis vectors
    vec3   defocus_disk_u;        // Defocus disk horizontal radius
    vec3   defocus_disk_v;        // Defocus disk vertical radius
};

#endif
