#pragma once
#include "ray.h"
#include "material.h"

struct record_hit
{
	point3 c_point;
	vec3 c_normal;
	double c_t;
	bool face_to_outside;
	shared_ptr<material> p_material;
	inline void set_face(const ray& r, const vec3& out_normal)
	{
		face_to_outside = dot(r.direction(), out_normal) < 0;
		c_normal = face_to_outside ? out_normal : -out_normal;
	}
};

class hittable
{
public:
	virtual bool hit(const ray& r, double t_min, double t_max, record_hit& rec_h) const = 0;
};