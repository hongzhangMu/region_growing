//#define NIFTI
//#include <cimgext/cimgext.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp>
#include "pechin_wrap.h"

namespace po = boost::program_options;
namespace bf = boost::filesystem;
        
std::string in_file, out_file;
int thresh;
std::vector<int> seeds;

int
main(int ac, char* av[])
{
    std::cout << "Simple region growing" << std::endl;
    std::cout << "Raghav, Version 1.00 2016" << std::endl;
    std::cout << std::endl;

    try
    {
        po::options_description options_description("Allowed options");
        options_description.add_options()
            ("help,h", "produce help message")
            ("in,i", po::value<std::string>(&in_file), "the input file")
            ("out,o", po::value<std::string>(&out_file), "the output file")
            ("threshold,t", po::value<int>(&thresh)->default_value(0))
            ("seeds,s", po::value< std::vector<int> >(&seeds)->multitoken(),
		"seed points: x0, y0, z0, ... xn, yn, zn");
        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, options_description), vm);
        po::notify(vm);

        if(vm.count("help") || in_file == "")
        {
            std::cout << "Usage: options_description [options]" << std::endl;
            std::cout << options_description;
            return 0;
        }
// Load input NIFTI and obtain dimensions

   	cimg_library::CImg<int> in(in_file.c_str());
	int xDim = in._width;
	int yDim = in._height;
	int zDim = in._depth;
	double oldSeeds = seeds.size()/3;
	std::cout << oldSeeds << std::endl;
// Create seed mask	
	cimg_library::CImg<int> mask = in; //(xDim,yDim,zDim); 
	cimg_forXYZ(mask,x,y,z) mask(x,y,z) = 0;

	for(int i = 0; i < oldSeeds; i++)
	{
		int j = i*3;
		printf(" using seed: %d %d %d\n", seeds[j], seeds[j+1], seeds[j+2]);
		mask(seeds[j],seeds[j+1],seeds[j+2]) = 100; 
	}
//	int oldSeeds = 1;
	double newSeeds = 0;
	int intens = 0;
	int xNbr, yNbr, zNbr;
	while(oldSeeds != newSeeds)
	{
		oldSeeds = newSeeds;
		newSeeds = 0;
		cimg_forXYZ(in,x,y,z) 
			{
				if(mask(x,y,z) > 0) intens = in(x,y,z);
				for(int i = -1; i < 2; i++)
				{
					xNbr = x + i;
					if(xNbr < xDim)
					{
						for(int j = -1; j < 2; j++)
						{
							yNbr = y + j;
							if(yNbr < yDim)
							{
								for(int k = -1; k < 2; k++)
								{
									zNbr = z + k;
									if(zNbr < zDim)
									{
										if(intens >= thresh) //&& intens <= (thresh+5))
										{
											newSeeds = newSeeds + 1;
											mask(xNbr,yNbr,zNbr) = 100;
										}
									}
								}
							} 
						}
					}
				}
				
			}	
	std::cout << newSeeds << std::endl;
	}




//	cimg_forXYZ(in,x,y,z)
//		in(x,y,z) = 1*in(x,y,z);
//	mask.set_metadata(in);	
	if(out_file != "") mask.save(out_file.c_str());
    }
    catch(std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

