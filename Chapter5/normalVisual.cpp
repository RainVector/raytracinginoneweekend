#include <fstream>
#include <iostream>
#include "ray.h"
float hit_sphere(const vec3& center,const float radius, const ray& r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = float(2.0) * dot(r.direction(),oc);
	float c = dot(oc,oc) - radius * radius;

	float discriminant = b*b - 4 * a * c;
	if(discriminant < 0)
		return -1.0;
	else
		return (-b - sqrt(discriminant))/(2.0*a); // return the smallest \t
}
vec3 color(const ray& r)
{
	float t = hit_sphere(vec3(0,0,-1),0.5,r);
	if(t > 0.0)
	{
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0,0,-1));
		return 0.5 * vec3(N.x() + 1,N.y() + 1, N.z() +1);
	}	

	//background
	vec3 unit_direction =unit_vector(r.direction());
	t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0,1.0,1.0) + t * vec3(0.5,0.7,1.0);
}

int main(int argc, char* argv[])
{
	std::ofstream imageFile;
    imageFile.open("image.ppm",'w');
    if(!imageFile.is_open())
        std::cerr<<"Failed to open file: image.ppm"<<std::endl;

    int nx = 200, ny = 100;
	imageFile << "P3" << std::endl;
	imageFile << nx << " " << ny << std::endl;
	imageFile << 255 << std::endl;


	vec3 lower_left_corner(-2.0,-1.0,-1.0);
	vec3 horizontal(4.0,0.0,0.0);
	vec3 vertical(0.0,2.0,0.0);
	vec3 origin(0.0,0.0,0.0);

    for( int j = ny - 1; j >=0; j--)
        for( int i = 0; i < nx; i++)
        {
            float u = float(i)/float(nx);
            float v = float(j)/float(ny);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 col = color(r);
			int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            imageFile << ir <<" " << ig <<" " << ib <<std::endl;
        }
	
    imageFile.close();
    return 0;
}