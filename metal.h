#pragma once
#include "material.h"

class metal :public material
{
public:
	metal(vec3 a, double f):albedo(a), fuzz(f<1?f:1.0) {}
	virtual bool scatter(const ray& r_in, const record_hit& rec, color& attenuation, ray& scattered) const override
	{
		//��������������
		/*vec3 ray_direction = reflect(r_in.direction(), rec.c_normal);
		scattered = ray(rec.c_point, fuzz*ray_direction);*/
		//����ģ��ϵ����
		vec3 ray_direction = reflect(r_in.direction(), rec.c_normal);
		scattered = ray(rec.c_point, fuzz * random_in_sphere() + ray_direction);

		attenuation = this->albedo;
		return (dot(ray_direction, rec.c_normal)>0);
	}

public:
	vec3 albedo;
	//���ģ��ϵ�������ϵ��ͨ���ı䷴��ķ�����ʹ����ģ��
	double fuzz;
};