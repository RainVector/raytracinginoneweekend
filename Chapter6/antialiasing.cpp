#include <fstream>
#include <iostream>
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"

/*
* Generating random numbers between [0,1)
* https://www.oreilly.com/library/view/c-cookbook/0596007612/ch11s07.html 
*/
#include <cstdlib>
#include <ctime>
float floatRand() {
	return float(rand()) / (float(RAND_MAX) + 1.0);
}

vec3 color(const ray& r, hitable *world)
{
    hit_record rec;
    if(world->hit(r,0.0, FLT_MAX,rec))
    {
        return 0.5 * (vec3(1.0,1.0,1.0) + rec.normal);
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
    imageFile.open("multiobj.ppm",'w');
    if(!imageFile.is_open())
        std::cerr<<"Failed to open file: image.ppm"<<std::endl;

    int nx = 200, ny = 100, ns =100;
	imageFile << "P3" << std::endl;
	imageFile << nx << " " << ny << std::endl;
	imageFile << 255 << std::endl;

    camera cam;

    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1),0.5);
    list[1] = new sphere(vec3(0,-100.5,-1),100);

	hitable_list *world = new hitable_list(list,2);

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
			    col += color(r,world);
            }
            col /= float(ns);
			int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            imageFile << ir <<" " << ig <<" " << ib <<std::endl;
        }
	
    imageFile.close();
    return 0;
}