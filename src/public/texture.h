#ifndef TEXTURE_H
#define TEXTURE_H

#include "includes.h"

#include "rtw_stb_image.h"

class texture
{
public:
    virtual ~texture() = default;

    virtual color value(double u, double v, const point3 &p) const = 0;
};

class solid_color_texture final : public texture
{
public:
    explicit solid_color_texture(const color &albedo) : albedo(albedo) {}

    solid_color_texture(const double red, const double green, const double blue) : solid_color_texture(color(red, green, blue)) {}

    color value(double u, double v, const point3 &p) const override { return albedo; }

private:
    color albedo;
};

class checker_texture final : public texture
{
public:
    checker_texture(const double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
        : inv_scale(1.0 / scale),
          even(std::move(even)),
          odd(std::move(odd)) {}

    checker_texture(const double scale, const color &c1, const color &c2)
        : inv_scale(1.0 / scale),
          even(make_shared<solid_color_texture>(c1)),
          odd(make_shared<solid_color_texture>(c2)) {}

    color value(const double u, const double v, const point3 &p) const override
    {
        const auto xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
        const auto yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
        const auto zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

        const bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double              inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

class image_texture final : public texture
{
public:
    explicit image_texture(const char *filename) : image(filename) {}

    color value(double u, double v, const point3 &p) const override
    {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) return {0, 1, 1};

        // Clamp input texture coordinates to [0, 1] x [1, 0]
        u = interval(0, 1).clamp(u);
        v = 1.0 - interval(0, 1).clamp(v); // Flip v to image coordinates

        const auto i     = static_cast<int>(u * image.width());
        const auto j     = static_cast<int>(v * image.height());
        const auto pixel = image.pixel_data(i, j);

        constexpr auto color_scale = 1.0 / 255.0;

        return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
    }

private:
    rtw_image image;
};

#endif
