#ifndef MATERIAL_H
#define MATERIAL_H
#include "ray.h"
#include "utility.h"

class material{
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian: public material
{
    public:
        lambertian(const vec3& a): albedo(a){}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
        {
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = ray(rec.p, target-rec.p);
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
};

class metal : public material
{
    public:
		metal(const vec3& a) : albedo(a) { }
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
        {
            vec3 reflected = reflect(unit_vector(r_in.direction()),rec.normal);
            scattered = ray(rec.p, reflected );
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal)>0);
        }

        vec3 albedo;
};
// add fuzz effects
class fuzzmetal : public material
{
public:
	fuzzmetal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal)>0);
	}

	vec3 albedo;
	float fuzz;
};

class dielectric : public material{
    public:
        dielectric(float ri): ref_idx(ri) {}
	    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
        {
            vec3 outward_normal;
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            float ni_over_nt;
			// blue part = 0.0  the left ball with light green below and gree color upper
			// blue part = 1.0. the left ball with background color below and green color upper
            attenuation = vec3(1.0,1.0,0.0);
            vec3 refracted;
            if(dot(r_in.direction(), rec.normal) > 0)
            {
                outward_normal = -rec.normal;
                ni_over_nt = ref_idx;
            }
            else
            {
                outward_normal = rec.normal;
                ni_over_nt = 1.0/ref_idx;
            }

            if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
            {
                scattered = ray(rec.p, refracted);
            }
            else
            {
                scattered = ray(rec.p, reflected);
                return false;
            }
            return true;
        }

        float ref_idx;
};

class glass : public material {
public:
	glass(float ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		float reflect_prob;
		float cosine;
		if (dot(r_in.direction(), rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}

		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, ref_idx);
		}
		else
		{
			scattered = ray(rec.p, reflected);
			reflect_prob = 1.0;
		}

		if (floatRand() < reflect_prob)
		{
			scattered = ray(rec.p, reflected);
		}
		else 
		{
			scattered = ray(rec.p, refracted);
		}
		return true;
	}

	float ref_idx;
};
#endif