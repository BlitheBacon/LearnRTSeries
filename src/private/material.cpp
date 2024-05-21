#include "material.h"

#include "hittable.h"


bool material::scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const { return false; }

lambertian::lambertian(const color &albedo) : tex(make_shared<solid_color_texture>(albedo)) {}

lambertian::lambertian(const shared_ptr<texture> &tex) : tex(tex) {}

bool lambertian::scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
{
    auto scatter_direction = rec.normal + random_unit_vector();

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero()) scatter_direction = rec.normal;

    scattered   = ray(rec.p, scatter_direction, r_in.time());
    attenuation = tex->value(rec.u, rec.v, rec.p);

    return true;
}

metal::metal(const color &albedo, const double fuzz)
    : albedo(albedo),
      fuzz(fuzz < 1 ? fuzz : 1) {}

bool metal::scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
{
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    reflected      = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered      = ray(rec.p, reflected, r_in.time());
    attenuation    = albedo;

    return (dot(scattered.direction(), rec.normal) > 0);
}

dielectric::dielectric(const double refraction_index) : refraction_index(refraction_index) {}

bool dielectric::scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
{
    attenuation     = color(1.0, 1.0, 1.0);
    const double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

    const vec3   unit_direction = unit_vector(r_in.direction());
    const double cos_theta      = fmin(dot(-unit_direction, rec.normal), 1.0);
    const double sin_theta      = sqrt(1.0 - (cos_theta * cos_theta));

    const bool cannot_refract = ri * sin_theta > 1.0;
    vec3       direction;

    if (cannot_refract || reflectance(cos_theta, ri) > random_double()) direction = reflect(unit_direction, rec.normal);
    else direction                                                                = refract(unit_direction, rec.normal, ri);

    scattered = ray(rec.p, direction, r_in.time());

    return true;
}

double dielectric::reflectance(const double cosine, const double refraction_index)
{
    // Use Schlick's approximation for reflectance.
    auto r0 = ((1 - refraction_index) / (1 + refraction_index));
    r0      = r0 * r0;

    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
