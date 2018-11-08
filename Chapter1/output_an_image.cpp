/*
* Write an PPM image to file
* PPM image formate:
P3
# The P3 means colors are in ASCII, then 3 columns and 2 rows
# then 255 for max color, then RGB triplets
#
3 2
255
255   0 0 	  0 255   0		0 0 255
255 255 0   255 255 255     0 0   0
*/

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
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            imageFile << ir <<" " << ig <<" " << ib <<std::endl;
        }
	
    imageFile.close();
    return 0;
}