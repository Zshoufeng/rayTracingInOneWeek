#pragma once
#include "hittable.h"

class sphere:public hittable
{
public:
	sphere(point3 center, double radius, shared_ptr<material> p):c_center(center), c_radius(radius), p_material(p) {}
	virtual bool hit(const ray& r, double t_min, double t_max, record_hit& rec_h) const;

	point3 center() { return c_center; }
	double radius() { return c_radius; }

private:
	point3 c_center;
	double c_radius;
	shared_ptr<material> p_material;
};

bool sphere::hit(const ray& r, double t_min, double t_max, record_hit& rec_h) const
{
	vec3 oc = r.origin() - c_center;
	double a = dot(r.direction(), r.direction());
	double half_b = dot(r.direction(), oc);
	double c = dot(oc, oc) - c_radius * c_radius;
	double discrinant = half_b * half_b - a * c;
	if (discrinant < 0)
	{
		return false;
	}

	double t1 = (-half_b - sqrt(discrinant)) / a;
	if (t1 < t_min || t1 > t_max)
	{
		t1 = (-half_b + sqrt(discrinant)) / a;
		if (t1 < t_min || t1 > t_max)
		{
			return false;
		}
	}

	rec_h.c_t = t1;
	rec_h.c_point = r.at(t1);
	//指向对应材质的球体
	rec_h.p_material = this->p_material;

	vec3 out_normal = (rec_h.c_point - c_center) / c_radius;
	rec_h.set_face(r, out_normal);

	return true;
}