#pragma once
#include "material.h"

class metal :public material
{
public:
	metal(vec3 a, double f):albedo(a), fuzz(f<1?f:1.0) {}
	virtual bool scatter(const ray& r_in, const record_hit& rec, color& attenuation, ray& scattered) const override
	{
		//金属的向量反射
		/*vec3 ray_direction = reflect(r_in.direction(), rec.c_normal);
		scattered = ray(rec.c_point, fuzz*ray_direction);*/
		//加入模糊系数后
		vec3 ray_direction = reflect(r_in.direction(), rec.c_normal);
		scattered = ray(rec.c_point, fuzz * random_in_sphere() + ray_direction);

		attenuation = this->albedo;
		return (dot(ray_direction, rec.c_normal)>0);
	}

public:
	vec3 albedo;
	//添加模糊系数，这个系数通过改变反射的方向来使球体模糊
	double fuzz;
};