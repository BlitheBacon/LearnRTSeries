# Formulae

### Ray

`P (t) = A + tb`\
*P* is a 3D position along a line in 3D\
*A* is the ray origin\
*b* is the ray direction\
*t* is a real number (double) such that P(t) moves the point along the ray

2. Linear Interpolation (LERP)
   blendedValue = (1 - a) * startValue + a * endValue
   WHERE 0 <= a <= 1.0
3. Spheres
   -- 3.1 A given point on the surface of a sphere | x^2 + y^2 + z^2 = r^2
   -- 3.2 A given point inside a sphere | x^2 + y^2 + z^2 < r^2
   -- 3.3 A given point outside a sphere | x^2 + y^2 + z^2 > r^2
   -- 3.4 The sphere center at an arbitrary point (C_x1, C_y1, C_z) | (C_x - x)^2 + (C_y - y)^2 + (C_z - z)^2 = r^2

4. Vector from arbitrary point P = (x, y, z) to arbitrary center C = (C_x1, C_y1, C_z) is (C - P)
   Using the dot product definition, the equation of a sphere in vector form can be rewritten as  (C - P) * (C - P) =
   r^2
   */~~~~