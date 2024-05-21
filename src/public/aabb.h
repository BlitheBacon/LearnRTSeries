#ifndef AABB_H
#define AABB_H

#include "includes.h"

/// Axis-Aligned Bounding Box
/// @details A method for determining bounds intersection. This class utilizes the "slab" method which
/// states that an n-dimensional AABB is just the intersection of n axis-aligned intervals, often called "slabs."
class aabb
{
public:
    interval x, y, z;

    /// @details Takes an interval set or a point set. Intervals are assigned as passed. Points are made into sets, ensuring min
    /// and max are assigned the lowest and highest values, respectively.
    aabb() = default;

    /// @details Axis-Aligned Bounding Box. Accepts 3 intervals that are passed by const reference.
    aabb(const interval &x, const interval &y, const interval &z);

    /// @details Axis-Aligned Bounding Box. Accepts 2 point3s that are passed by const reference.
    /// Both points are treated as extremes for the bounding box, meaning that min/max orderx isn't required.
    aabb(const point3 &a, const point3 &b);

    /// @details Axis-Aligned Bounding Box. Accepts 2 bounding boxes that are passed by const reference.
    /// The X, Y, and Z values of both boxes are passed to an X, Y, and Z interval.
    aabb(const aabb &box0, const aabb &box1);


    const interval &axis_interval(const int n) const;

    bool hit(const ray &r, interval ray_t) const;

    int longest_axis() const;

    static const aabb empty, universe;
};

constexpr aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
constexpr aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif
