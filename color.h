#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3;

inline double linear_to_gamma(double linear_component)
{
	if (linear_component > 0)
	{
		return sqrt(linear_component);
	}
	return 0;
}

void write_color(std::ostream& out, const color& pixel_color)
{
	// Converting RGB values from linear to gamma space
	auto r = pixel_color.x();
	r = linear_to_gamma(r);

	auto g = pixel_color.y();
	g = linear_to_gamma(g);

	auto b = pixel_color.z();
	b = linear_to_gamma(b);

	// Translate the [0,1] component values to the byte range [0,255].
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	// Write out the pixel color components.
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
