#ifndef INTERVAL_H
#define INTERVAL_H

class interval
{
public:
    double min, max;

    // Default interval is empty
    interval()
        : min(+infinity),
          max(-infinity) {}

    interval(double min, double max)
        : min(min),
          max(max) {}

    interval(const interval &a, const interval &b)
    {
        // Create the interval tightly enclosing the two input intervals.
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    double size() const { return max - min; }

    bool contains(double x) const { return min <= x && x <= max; }

    bool surrounds(double x) const { return min < x && x < max; }

    double clamp(double x) const
    {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    // Pads the interval by a given delta.
    interval expand(double delta) const
    {
        auto padding = delta / 2;
        return {min - padding, max + padding};
    }

    static const interval empty, universe;
};

const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif
