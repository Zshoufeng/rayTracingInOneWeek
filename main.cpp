#include <iostream>
using namespace std;

#include "sphere.h"
#include "hittable_list.h"
#include "color.h"
#include "camera.h"
#include "lambertion.h"
#include "metal.h"
#include "dielectric.h"

//float hit_sphere(const ray& r, const vec3& center, float radius)
//{
//    vec3 oc = r.origin() - center;
//    float a = dot(r.direction(), r.direction());
//    float half_b = dot(oc, r.direction());
//    float c = dot(oc, oc) - radius * radius;
//    auto discriminant = half_b * half_b - a * c;
//    if (discriminant < 0)
//    {
//        return -1;
//    }
//    float t1 = (-half_b - sqrt(discriminant)) / a;
//    if (t1 > 0)
//    {
//        return t1;
//    }
//    float t2 = (-half_b + sqrt(discriminant)) / a;
//    return t2;
//}
//
//vec3 ray_color(ray& r)
//{
//    vec3 _center(0.0, 0.0, -1.0);
//    float t_t = hit_sphere(r, _center, 0.5);
//    if (t_t > 0)
//    {
//        vec3 t_point = r.at(t_t);
//        vec3 t_unit_normal = unit_vector(t_point - _center);
//        if (dot(t_unit_normal, r.origin() - _center) < 0)
//        {
//            return vec3(0, 0, 0);
//        }
//        return 0.5 * vec3(t_unit_normal.x()+1, t_unit_normal.y() + 1, t_unit_normal.z() + 1);
//    }
//    vec3 unit_direction = unit_vector(r.direction());
//    auto t = (unit_direction.y() + 1.0) * 0.5;
//    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.3, 0.7, 1.0);
//}

////对多个物体求交，然后取最近的物体，用法线来生成颜色
//vec3 ray_color(const ray& r, const hittable& object)
//{
//    record_hit t_rec;
//    double t_min = 0, t_max = DBL_MAX;
//    if (object.hit(r, t_min, t_max, t_rec))
//    {
//        return 0.5*(t_rec.c_normal + color(1, 1, 1));
//    }
//    vec3 unit_direction = unit_vector(r.direction());
//    auto t = (unit_direction.y() + 1.0) * 0.5;
//    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.3, 0.7, 1.0);
//}

//加入漫反射的随机方向
//vec3 ray_color(const ray& r, const hittable& object, int depth)
//{
//    if (depth <= 0)
//    {
//        return color(0, 0, 0);
//    }
//    record_hit t_rec;
//    double t_min = 0.001, t_max = DBL_MAX;
//    if (object.hit(r, t_min, t_max, t_rec))
//    {
//        /*auto t_p = t_rec.c_point + t_rec.c_normal + random_in_sphere();*/
//        /*auto t_p = t_rec.c_point + t_rec.c_normal + random_unit_vector();*/
//        auto t_p = t_rec.c_point + t_rec.c_normal + random_in_hemisphere(t_rec.c_normal);
//        return 0.5 * ray_color(ray(r.origin(), t_p - t_rec.c_point), object, --depth);
//    }
//    vec3 unit_direction = unit_vector(r.direction());
//    auto t = (unit_direction.y() + 1.0) * 0.5;
//    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
//}


//创建一个随机的世界球类
hittable_list random_scene()
{
    hittable_list world;
    auto ground_material = make_shared<lambertion>(color(0.5,0.5,0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0),1000, ground_material));

    for (int i = -10; i < 10; ++i)
    {
        for (int j = -10; j < 10; ++j)
        {
            double choose_num = vec3::random_double();
            // 随机当前小球的中心位置
            point3 center(i + 0.9 * vec3::random_double(), 0.2, j + 0.9 * vec3::random_double());
            // 如果中心位置不太靠世界坐标原点左右
            if ((center - point3(4, 0.2, 0)).length() > 0.9) 
            {
                shared_ptr<material> sphere_material;
                //散射
                if (choose_num < 0.8)
                {
                    vec3 t_albedo = vec3::random() * vec3::random();
                    sphere_material = make_shared<lambertion>(t_albedo);
                }
                //反射
                else if (choose_num < 0.95)
                {
                    vec3 t_albedo = vec3::random() * vec3::random();
                    double t_fuzz = vec3::random_double(0,0.5);
                    sphere_material = make_shared<metal>(t_albedo, t_fuzz);
                }
                //折射
                else 
                {
                    sphere_material = make_shared<dielectric>(1.5);
                }
                world.add(make_shared<sphere>(center, 0.2, sphere_material));
            }
        }
    }

    // 玻璃大球
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    // 散射大球
    auto material2 = make_shared<lambertion>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    // 金属大球
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

//加入材质类
vec3 ray_color(const ray& r, const hittable& object, int depth)
{
    if (depth <= 0)
    {
        return color(0, 0, 0);
    }
    record_hit t_rec;
    double t_min = 0.001, t_max = DBL_MAX;
    if (object.hit(r, t_min, t_max, t_rec))
    {
        color albedo;
        ray scattered;
        if (t_rec.p_material->scatter(r, t_rec, albedo, scattered))
        {
            return albedo * ray_color(scattered, object, --depth);
        }
        return color(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = (unit_direction.y() + 1.0) * 0.5;
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {

    float multi_sample_w[4] = { 0.25,0.25,0.75,0.75 };
    float multi_sample_h[4] = { 0.25,0.75,0.25,0.75 };

    // Image
    //const auto aspect_ratio = 16.0 / 9.0;
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    //World
    //材质球 in world
    //auto material_ground = make_shared<lambertion>(color(0.8, 0.8, 0));
    ///*auto material_center = make_shared<lambertion>(color(0.7, 0.3, 0.3));
    //auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8),0.3);*/
    ////auto material_center = make_shared<dielectric>(1.5);
    //auto material_center = make_shared<lambertion>(color(0.1, 0.2, 0.5));
    //auto material_left = make_shared<dielectric>(1.5);
    //auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2),0.0);
    //hittable_list world;
    //world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
    //world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
    //world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left));
    //world.add(make_shared<sphere>(point3(-1, 0, -1), -0.4, material_left));
    //world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, material_right));

    hittable_list world = random_scene();

    // Camera
    /*vec3 origin(0.0, 0.0, 0.0);
    auto view_height = 2.0;
    auto view_width = view_height * aspect_ratio;
    auto focus_distance = 1.0;

    vec3 viewport_wid(view_width, 0.0, 0.0);
    vec3 viewport_hei(0.0, view_height, 0.0);
    vec3 left_bottom_corner = origin - viewport_hei/2 - viewport_wid/2 - vec3(0.0, 0.0, focus_distance);*/

    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    double fov = 20;
    //auto dist_to_focus = (lookfrom - lookat).length();
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera t_camera(lookfrom, lookat, vup, fov, aspect_ratio, aperture, dist_to_focus);

    // Render
    int samples = 50;
    int max_depth = 50;
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0.0, 0.0, 0.0);
            for (int k = 0; k < samples; k++)
            {
                float u = (double(i) + vec3::random_double()) / (double)(image_width - 1);
                float v = (double(j) + vec3::random_double()) / (double)(image_height - 1);
                ray r = t_camera.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples);
        }
    }
    std::cerr << "\nDone.\n";
}