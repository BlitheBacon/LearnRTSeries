#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
    ray() = default;

    ray(const point3 &origin, const vec3 &direction)
        : orig(origin),
          dir(direction),
          tm(0) {}

    ray(const point3 &origin, const vec3 &direction, const double time)
        : orig(origin),
          dir(direction),
          tm(time) {}

    const point3 &origin() const { return orig; }

    const point3 &direction() const { return dir; }

    point3 at(const double t) const { return orig + (t * dir); }

    double time() const { return tm; }

private:
    point3 orig;
    vec3   dir;
    double tm;
};

#endif
