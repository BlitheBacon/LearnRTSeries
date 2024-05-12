#ifndef INCLUDES_H
#define INCLUDES_H

// STL Includes
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

// C++ STD Usings
using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants
const double     infinity = std::numeric_limits<double>::infinity();
constexpr double pi       = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) { return degrees * pi / 180; }

inline double random_double()
{
    // Returns a random real number in [0, 1).
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937                           generator;
    return distribution(generator);
}

inline double random_double(double min, double max)
{
    // Returns a random real number in [min, max).
    return (min + (max - min) * random_double());
}

inline int random_int(int min, int max)
{
    // Return a random integer in [min, max].
    return static_cast<int>(random_double(min, max + 1));
}

// Includes
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
