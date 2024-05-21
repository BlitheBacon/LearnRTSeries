#ifndef HEADER_H
#define HEADER_H

#include "includes.h"

class texture
{
public:
    virtual ~texture() = default;

    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color_texture final : public texture
{
public:
    explicit solid_color_texture(const color& albdeo) : albedo(albedo) {}
    solid_color_texture(const double red, const double green, const double blue) : solid_color_texture(color(red, green, blue)) {}

    color value(double u, double v, const point3& p) const override
    {
        return albedo;
    }

private:
    color albedo;
};

#endif
