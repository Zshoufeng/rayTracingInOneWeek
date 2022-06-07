#pragma once
#include "material.h"

class lambertion :public material
{
public:
	lambertion(vec3 a):albedo(a) {}
	virtual bool scatter(const ray& r_in, const record_hit& rec, color& attenuation, ray& scattered) const override
	{
		vec3 scatter_direction = rec.c_normal + random_unit_vector();
		scattered = ray(rec.c_point, scatter_direction);
		attenuation = albedo;
		return true;
	}

public:
	vec3 albedo;
};