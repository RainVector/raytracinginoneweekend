#include <fstream>
#include <iostream>
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "utility.h"
#include "material.h"
vec3 color(const ray& r, hitable *world, int depth)
{
    hit_record rec;
    // fix the bug by set t = 0.001
    if(world->hit(r,0.001, FLT_MAX,rec))
    {
        ray scattered;
        vec3 attenuation;
        if(depth < 50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered))
        {
            return attenuation * color(scattered, world, depth+1);
        }
        else
        {
            return vec3(0,0,0);
        }
    }
    else
    {
        vec3 unit_direction =unit_vector(r.direction());
	    float t = 0.5 * (unit_direction.y() + 1.0);
	    return (1.0 - t) * vec3(1.0,1.0,1.0) + t * vec3(0.5,0.7,1.0);
    }
}

int main(int argc, char* argv[])
{
	std::ofstream imageFile;
    imageFile.open("camera.ppm",'w');
    if(!imageFile.is_open())
        std::cerr<<"Failed to open file: image.ppm"<<std::endl;

    int nx = 200, ny = 100, ns =100;
	imageFile << "P3" << std::endl;
	imageFile << nx << " " << ny << std::endl;
	imageFile << 255 << std::endl;

    vec3 lookfrom(3,3,2);
    vec3 lookat(0,0,-1);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperture = 2.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny),aperture,dist_to_focus);


	hitable *list[4];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));

	hitable_list *world = new hitable_list(list, 4);

	srand(static_cast<unsigned int>(clock())); // for generating random numbers

    for( int j = ny - 1; j >=0; j--)
        for( int i = 0; i < nx; i++)
        {
            vec3 col(0,0,0);
            for(int s = 0; s < ns; s++)
            {
                float u = float(i + floatRand())/float(nx);
                float v = float(j + floatRand())/float(ny);
                ray r = cam.get_ray(u,v);
			    col += color(r,world,0);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]),sqrt(col[1]),sqrt(col[2]));
			int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            imageFile << ir <<" " << ig <<" " << ib <<std::endl;
        }
	
    imageFile.close();
    return 0;
}