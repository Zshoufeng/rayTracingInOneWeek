#pragma once
#include "hittable.h"
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable
{
public:
	hittable_list(){}
	hittable_list(shared_ptr<hittable> object) { c_objects.push_back(object); }
	virtual bool hit(const ray& r, double t_min, double t_max, record_hit& rec_h) const;
	void add(shared_ptr<hittable> object) { c_objects.push_back(object); }
	void clear() { c_objects.clear(); }
private:
	std::vector<shared_ptr<hittable>> c_objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, record_hit& rec_h) const
{
	record_hit t_rec;
	double closest_t = t_max;
	bool hit_anything = false;
	for (const auto& object : c_objects)
	{
		if (object->hit(r, t_min, closest_t, t_rec))
		{
			closest_t = t_rec.c_t;
			hit_anything = true;
			rec_h = t_rec;
		}
	}
	return hit_anything;
}
