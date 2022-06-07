#pragma once
#include "vec3.h"

#include <iostream>

double clamp(double num, double tmin, double tmax)
{
    double n = std::max(num, tmin);
    return std::min(n, tmax);
}

void write_color(std::ostream& out, color pixel_color, int samples) {
    // Write the translated [0,255] value of each color component.
    double scale = 1.0 / samples;
    double r = sqrt(scale * pixel_color.x());
    double g = sqrt(scale * pixel_color.y());
    double b = sqrt(scale * pixel_color.z());
    out << static_cast<int>(255.999 * clamp(r,0.0,0.999)) << ' '
        << static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << '\n';
}