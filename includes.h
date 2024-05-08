#ifndef INCLUDES_H
#define INCLUDES_H

// STL Includes
#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>

// C++ STD Usings
using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180;
}

inline double random_double()
{
	// Returns a random real in [0, 1).
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double min, double max)
{
	// Returns a random real in [min, max).
	static std::uniform_real_distribution<double> distribution(min, max);
	static std::mt19937 generator;
	return distribution(generator);
}

// Includes
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif