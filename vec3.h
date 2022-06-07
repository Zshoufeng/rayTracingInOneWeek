#pragma once
#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public:
    vec3() : e{ 0,0,0 } {}
    vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1 / t;
    }

    inline static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static double random_double()
    {
        return rand()/ (RAND_MAX + 1.0);
    }

    inline static double random_double(double dmin, double dmax)
    {
        return dmin + (dmax - dmin) * random_double();
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

public:
    double e[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

inline vec3 operator+(const vec3& a, const vec3& b)
{
    return vec3(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

inline vec3 operator-(const vec3& a, const vec3& b)
{
    return vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

inline vec3 operator*(const vec3& a, const vec3& b)
{
    return vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

inline vec3 operator*(double t, const vec3& a)
{
    return vec3(a[0] * t, a[1] * t, a[2] * t);
}

inline vec3 operator*(const vec3& a, double t)
{
    return t * a;
}

inline vec3 operator/(const vec3& a, double t)
{
    return (1/t) * a;
}

inline double dot(const vec3& a, const vec3& b)
{
    return a.e[0] * b.e[0] + a.e[1] * b.e[1] + a.e[2] * b.e[2];
}

inline vec3 cross(const vec3& a, const vec3& b)
{
    double n1 = a.e[1] * b.e[2] - a.e[2] * b.e[1];
    double n2 = a.e[2] * b.e[0] - a.e[0] * b.e[2];
    double n3 = a.e[0] * b.e[1] - a.e[1] * b.e[0];
    return vec3(n1, n2, n3);
}

inline vec3 unit_vector(const vec3& a)
{
    return a / a.length();
}

//加入漫反射物体,生成球内的任意一个方向
vec3 random_in_sphere()
{
    while (1)
    {
        vec3 p = vec3(vec3::random_double(-1.0, 1.0), vec3::random_double(-1.0, 1.0), vec3::random_double(-1.0, 1.0));
        if (p.length() >= 1.0)
        {
            continue;
        }
        return p;
    }
}

vec3 random_unit_vector()
{
    double a = vec3::random_double(0, 2 * 3.1415);
    double z = vec3::random_double(-1, 1);
    double r = sqrt(1 - z * z);
    return vec3(r * cos(a), r * sin(a), z);
}

vec3 random_in_hemisphere(const vec3& normal)
{
    vec3 vec = random_in_sphere();
    if (dot(vec, normal) > 0)
    {
        return vec;
    }
    else
    {
        return -vec;
    }
}

//反射
vec3 reflect(const vec3& v, const vec3& n) 
{
    return v - 2 * dot(v, n) * n;
}

//折射,uv n 均为单位向量
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
    double cos_num = fmin(dot(-uv, n), 1.0);
    vec3 uv_pers = etai_over_etat * (uv + cos_num * n);
    vec3 uv_parall = -sqrt(fabs(1.0 - dot(uv_pers, uv_pers))) * n;
    return uv_parall + uv_pers;
}

//在一个单位圆盘随机生成一个点
vec3 random_in_unit_disk()
{
    return vec3(vec3::random_double(0, 1), vec3::random_double(0, 1), vec3::random_double(0, 1));
}