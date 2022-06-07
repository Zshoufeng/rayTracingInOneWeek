#pragma once
#include "vec3.h"

class ray
{
public:
	ray() {}
	ray(const point3& p, const vec3& d):c_origin(p),c_dir(d){}

	point3 origin() const { return c_origin; }
	vec3 direction() const { return c_dir; }
	point3 at(double t) const { return c_origin + t*c_dir; }

private:
	point3 c_origin;
	vec3 c_dir;
};
