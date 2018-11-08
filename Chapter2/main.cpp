#include "vec3.h"
#include <iostream>
#include <fstream>

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
    for( int j = ny - 1; j >=0; j--)
        for( int i = 0; i < nx; i++)
        {
            float r = float(i)/float(nx);
            float g = float(j)/float(ny);
            float b = 0.2;
            vec3 col(r,g,b);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            imageFile << ir <<" " << ig <<" " << ib <<std::endl;
        }
	
    imageFile.close();
    return 0;
}
