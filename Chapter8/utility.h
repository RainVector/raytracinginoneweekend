#ifndef UTILITY_H
#define UTILITY_H
/*
* Generating random numbers between [0,1)
* https://www.oreilly.com/library/view/c-cookbook/0596007612/ch11s07.html 
*/
#include <cstdlib>
#include <ctime>

#define M_PI 3.141592653

float floatRand() {
	return float(rand()) / (float(RAND_MAX) + 1.0);
}

vec3 random_in_unit_sphere()
{
    vec3 p;
    do {
        p = 2.0 * vec3(floatRand(),floatRand(),floatRand()) - vec3(1.0,1.0,1.0);
	} while (p.squared_length() >= 1.0);
    return p;
}

vec3 reflect(const vec3& v, const vec3& n) 
{
	return v - 2 * dot(v, n) * n;
}
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
    vec3 uv = unit_vector(v);
    float dt = dot(uv,n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if(discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n*dt)  - n* sqrt(discriminant);
        return true;
    }
    else
        return false;
}
/*
	The real glass has reflectivity that varies with angle
	This is the polynomial approximation by Christophe Schlic
*/
float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}
#endif