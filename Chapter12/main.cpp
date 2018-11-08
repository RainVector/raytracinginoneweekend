#include <fstream>
#include <iostream>
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "utility.h"
#include "material.h"

hitable *random_scene()
{
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++)
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = floatRand();
			vec3 center(a + 0.9*floatRand(), 0.2, b + 0.9*floatRand());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{
					//diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(floatRand()*floatRand(), floatRand()*floatRand(), floatRand()*floatRand())));
				}
				else if (choose_mat < 0.95)
				{
					//metal
					list[i++] = new sphere(center, 0.2,
						new fuzzmetal(vec3(0.5*(1 + floatRand()), 0.5*(1 + floatRand()), 0.5*(1 + floatRand())), 0.5*floatRand()));
				}
				else
				{
					//glass
					list[i++] = new sphere(center, 0.2, new glass(1.5));
				}
			}
		}
	list[i++] = new sphere(vec3( 0, 1, 0), 1.0, new glass(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3( 4, 1, 0), 1.0, new fuzzmetal(vec3(0.7, 0.6, 0.5), 0.0));
	return new hitable_list(list, i);
}

vec3 color(const ray& r, hitable *world, int depth)
{
	hit_record rec;
	// fix the bug by set t = 0.001
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main(int argc, char* argv[])
{
	std::ofstream imageFile;
	imageFile.open("bingo.ppm", 'w');
	if (!imageFile.is_open())
		std::cerr << "Failed to open file: image.ppm" << std::endl;

	int nx = 200, ny = 100, ns = 100;
	imageFile << "P3" << std::endl;
	imageFile << nx << " " << ny << std::endl;
	imageFile << 255 << std::endl;

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10;
	float aperture = 0.1;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);

	hitable *world = random_scene();

	srand(static_cast<unsigned int>(clock())); // for generating random numbers

	for (int j = ny - 1; j >= 0; j--)
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + floatRand()) / float(nx);
				float v = float(j + floatRand()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			imageFile << ir << " " << ig << " " << ib << std::endl;
		}

	imageFile.close();
	return 0;
}