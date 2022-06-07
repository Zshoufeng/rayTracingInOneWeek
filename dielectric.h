#pragma once
#include "material.h"

class dielectric :public material
{
public:
	dielectric(double a) :ir(a) {}
	virtual bool scatter(const ray& r_in, const record_hit& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double refract_ratio = rec.face_to_outside ? (1.0 / ir) : ir;
		vec3 unit_in_dir = unit_vector(r_in.direction());
		double cosnum = dot(unit_in_dir, rec.c_normal);
		double sinnum = sqrt(1 - cosnum * cosnum);
		bool cannotRefract = sinnum*refract_ratio > 1 ;

		vec3 refract_dir;
		if (cannotRefract || reflectance(cosnum,refract_ratio) > vec3::random_double())
		{
			//全反射
			refract_dir = reflect(unit_in_dir, rec.c_normal);
		}
		else
		{
			//能折射
			refract_dir = refract(unit_in_dir, rec.c_normal, refract_ratio);
		}
		scattered = ray(rec.c_point, refract_dir);
		return true;
	}

public:
	double ir;

private:
	//模拟玻璃发生折射的概率随入射角会发生改变
	//用Schlick 近似估算
	static double reflectance(double cosine, double ref_idx) {
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};