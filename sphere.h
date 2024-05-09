#ifndef SPHERE_H
#define SPHERE_H

#include <utility>

#include "includes.h"

class sphere :
	public hittable
{
 public:
	// Stationary Sphere
	sphere(const point3& center, double radius, shared_ptr<material> mat)
		: center1(center), radius(fmax(0, radius)), mat(std::move(mat)), is_moving(false)
	{}

	// Moving Shpere
	sphere(const point3& center1, const point3& center2, double radius, shared_ptr<material> mat)
		: center1(center1), radius(fmax(0, radius)), mat(std::move(mat)), is_moving(true)
	{
		center_vec = center2 - center1;
	}

	bool hit(const ray& r, const interval ray_t, hit_record& rec) const override
	{
		const point3 center = is_moving ? sphere_center(r.time()) : center1;
		const vec3 oc = center - r.origin();
		const auto a = r.direction().length_squared();
		const auto h = dot(r.direction(), oc);
		const auto c = oc.length_squared() - radius * radius;

		const auto discriminant = h * h - a * c;
		if (discriminant < 0)
		{
			return false;
		}

		const auto sqrtd = sqrt(discriminant);

		// Find the nearest root that lies the acceptable range
		auto root = (h - sqrtd) / a;
		if (!ray_t.surrounds(root))
		{
			root = (h + sqrtd) / a;
			if (!ray_t.surrounds(root))
			{
				return false;
			}
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		const vec3 outward_normal = (rec.p - center1) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat = mat;

		return true;
	}

 private:
	point3 center1;
	double radius;
	shared_ptr<material> mat;
	bool is_moving{};
	vec3 center_vec;

	point3 sphere_center(const double time) const {
		// Linearly interpolate from center1 to center2 according to time, where t=0 yields
		// center1, and t=1 yields center2
		return center1 + (time * center_vec);
	}
};

#endif
