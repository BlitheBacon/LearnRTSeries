#ifndef BVH_H
#define BVH_H

#include "includes.h"

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

// Bounding Volume Hierarchy
class bvh_node final : public hittable
{
public:
    explicit bvh_node(hittable_list list);

    bvh_node(std::vector<shared_ptr<hittable> > &objects, size_t start, size_t end);

    bool hit(const ray &r, const interval ray_t, hit_record &rec) const override;

    aabb bounding_box() const override;

private:
    static bool box_compare(const shared_ptr<hittable> &a, const shared_ptr<hittable> &b, const int axis_index);

    static bool box_x_compare(const shared_ptr<hittable> &a, const shared_ptr<hittable> &b);

    static bool box_y_compare(const shared_ptr<hittable> &a, const shared_ptr<hittable> &b);

    static bool box_z_compare(const shared_ptr<hittable> &a, const shared_ptr<hittable> &b);

private:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb                 bbox;
};

#endif
