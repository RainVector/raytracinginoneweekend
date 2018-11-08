#ifndef HITABLE_H
#define HITABLE_H
#include "ray.h"

struct hit_record
{
    float t; 
    vec3 p; // hit point
    vec3 normal; 
};


class hitable
{
public:
    virtual bool hit(const ray& r,float t_min, float t_max, hit_record& rec) const = 0;
};

#endif