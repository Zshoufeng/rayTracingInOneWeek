#pragma once
#include "vec3.h"
#include "ray.h"

class camera
{
public:
	camera(point3 lookfrom, point3 lookat, vec3 upv, double fov, double aspect_ratio, double aperture, double focus_length);
	ray get_ray(double u, double v);
private:
	point3 c_origin;
	point3 c_left_low_corner;
	vec3 c_vertical;
	vec3 c_horizontal;

	double c_aspect_ratio;
	double c_focus_length;
	double c_fov;

	vec3 c_u;
	vec3 c_v;
	vec3 c_w;
	double c_len_radius;
};

camera::camera(point3 lookfrom, point3 lookat, vec3 upv, double fov, double aspect_ratio, double aperture, double focus_length)
{
	this->c_aspect_ratio = aspect_ratio;
	this->c_focus_length = focus_length;
	this->c_fov = fov;

	double angle = fov / 180 * 3.1415;
	double viewport_h = 2.0 * tan(angle/2) * focus_length;
	double viewport_w = viewport_h * aspect_ratio;

	this->c_w = unit_vector(lookfrom - lookat);
	this->c_u = unit_vector(cross(upv, c_w));
	this->c_v = unit_vector(cross(c_w, c_u));

	this->c_origin = lookfrom;
	this->c_vertical = viewport_w * c_u;
	this->c_horizontal = viewport_h * c_v;
	this->c_left_low_corner = c_origin - c_vertical/2 - c_horizontal/2 - c_focus_length*c_w;

	this->c_len_radius = aperture / 2;
}

ray camera::get_ray(double u, double v)
{
	vec3 t_v = c_len_radius * random_in_unit_disk();
	vec3 offset = t_v.x() * c_u + t_v.y() * c_v;
	return ray(c_origin + offset, c_left_low_corner + u * c_vertical + v * c_horizontal - c_origin - offset);
}