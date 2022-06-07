#pragma once
#include "ray.h"
struct record_hit;

class material
{
public:
	virtual bool scatter(const ray& r_in, const record_hit& rec, color& attenuation, ray& scattered) const = 0;
};