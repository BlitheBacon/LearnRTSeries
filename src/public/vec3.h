#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

/// 3-Dimensional Vector
class vec3
{
public:
    double e[3];

    vec3() : e{0, 0, 0} {}

    vec3(const double e0, const double e1, const double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }

    double y() const { return e[1]; }

    double z() const { return e[2]; }

    vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }

    double operator[](const int i) const { return e[i]; }

    double &operator[](const int i) { return e[i]; }

    vec3 &operator+=(const vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3 &operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(const double t) { return *this *= 1 / t; }

    double length() const { return sqrt(length_squared()); }

    double length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    bool near_zero() const
    {
        // Return true if the vector is close to zero in all dimensions.
        constexpr auto s = 1e-8;

        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

    static vec3 random() { return {random_double(), random_double(), random_double()}; }

    static vec3 random(const double min, const double max) { return {random_double(min, max), random_double(min, max), random_double(min, max)}; }
};


// Point 3 is an alias for vec3, which is useful for geometric clarity in the code.
using point3 = vec3;


// Vector Operator Overloads -------------------------------------------------------------------------------------------------------------------------

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) { return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2]; }

inline vec3 operator+(const vec3 &u, const vec3 &v) { return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]}; }

inline vec3 operator-(const vec3 &u, const vec3 &v) { return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]}; }

inline vec3 operator*(const vec3 &u, const vec3 &v) { return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]}; }

inline vec3 operator*(const double t, const vec3 &v) { return {t * v.e[0], t * v.e[1], t * v.e[2]}; }

inline vec3 operator*(const vec3 &v, const double t) { return t * v; }

inline vec3 operator/(const vec3 &v, const double t) { return (1 / t) * v; }


// Vector Math Methods -------------------------------------------------------------------------------------------------------------------------------
/// Dot Product
/// @details Performs a dot product operation on:
/// @param u a const reference to a 3-dimensional vector,
/// @param v a const reference to a 3-dimensional vector.
/// @return the dot product of u and v.
inline double dot(const vec3 &u, const vec3 &v) { return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]; }

/// Cross Product
/// @details Performs a cross product operation on:
/// @param u a const reference to a 3-dimensional vector,
/// @param v a const reference to a 3-dimensional vector.
/// @return the cross product of u and v.
inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

/// Unit Vector Normalization
/// @details Normalizes the length of a vector such that 0 <= v <= 1.
/// @param v a const reference to a 3-dimensional vector.
/// @return the normalized vector v.
inline vec3 unit_vector(const vec3 &v) { return v / v.length(); }


// Vector Randomize Methods --------------------------------------------------------------------------------------------------------------------------
/// Random Vector Within Unit Disk
/// @details Calculates a random value between -1 and 1 for both dimensions of a 2-dimensional vector, assigns the value to the x and y, and if the
/// length squared is less than 1, return the value.
/// @return The vector containing the random values.
inline vec3 random_in_unit_disk()
{
    while (true)
    {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1) return p;
    }
}

/// Random Vector Within Unit Sphere
/// @details Calculates a random value between -1 and 1 for all dimensions of a 3-dimensional vector; assigns the value to the x, y, and z; and if the
/// length squared is less than 1, return the value.
/// @return The vector containing the random values.
inline vec3 random_in_unit_sphere()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() < 1) return p;
    }
}

/// Random Unit Vector
/// @details Calculates a random value between -1 and 1 for all dimensions of a 3-dimensional vector; assigns the value to the x, y, and z; and if the
/// length squared is less than 1, return the value. This value is then passed to the Unit Vector method to normalize the vector.
/// @return The normalized vector.
inline vec3 random_unit_vector() { return unit_vector(random_in_unit_sphere()); }

/// Random Unit Vector On Unit Sphere
/// @details Calculates a random value between 0 and 1 for all dimensions of a 3-dimensional vector; assigns the value to the x, y, and z; and if the
/// length squared is less than 1, return the value.
/// @return The vector containing the random values.
inline vec3 random_on_hemisphere(const vec3 &normal)
{
    const vec3 on_unit_sphere = random_unit_vector();

    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    return -on_unit_sphere;
}


// Vector Angle Methods ------------------------------------------------------------------------------------------------------------------------------
/// Reflect Angle
/// @details r = d - 2(d * n) * n \n
/// Determines the direction of a ray reflected from a metallic, mirrored surface.
/// @param v A const reference to a 3-dimensional vector representing the ray of incidence. This may or may not be a
/// unit vector.
/// @param n A const reference to a 3-dimensional vector representing a unit vector of the surface normal.
/// @return A Vec3 of the angle of reflectance.
inline vec3 reflect(const vec3 &v, const vec3 &n) { return v - 2 * dot(v, n) * n; }

/// Refracted Angle
/// @details R' = R'_perp + R'_parallel \n
/// R'_perp = (eta / eta')(R + (-R * n) n) \n
/// R'_parallel = -sqrt(1 - abs(R'_perp)^2 n) \n
/// Determines the direction of a ray refracted through a dielectric.
/// @param uv A const reference to a 3-dimensional vector representing the angle of incidence.
/// @param n A const reference to a 3-dimensional vector representing a unit vector of the surface normal.
/// @param etai_over_etat A const double reference representing the refraction index (RI). RI = (eta_prime / eta).
/// @return A Vec3 of the angle of reflectance.
inline vec3 refract(const vec3 &uv, const vec3 &n, const double &etai_over_etat)
{
    const auto cos_theta      = fmin(dot(-uv, n), 1.0);
    const vec3 r_out_perp     = etai_over_etat * (uv + cos_theta * n);
    const vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;

    return r_out_perp + r_out_parallel;
}

#endif
