#ifndef MATERIAL_H
#define MATERIAL_H

#include "includes.h"

#include "texture.h"

class hit_record;

class material
{
public:
    virtual ~material() = default;

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const;
};

class lambertian final : public material
{
public:
    explicit lambertian(const color &albedo);

    explicit lambertian(const shared_ptr<texture> &tex);

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override;

private:
    color               albedo;
    shared_ptr<texture> tex;
};

class metal final : public material
{
public:
    metal(const color &albedo, const double fuzz);

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override;

private:
    color  albedo;
    double fuzz;
};

class dielectric final : public material
{
public:
    explicit dielectric(const double refraction_index);

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override;

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refraction_index;

    static double reflectance(const double cosine, const double refraction_index);
};
#endif
