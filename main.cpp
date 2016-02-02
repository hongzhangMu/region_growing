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

   	cimg_library::CImg<int> in(in_file.c_str());
	cimg_forXYZ(in,x,y,z)
		in(x,y,z) = 1*in(x,y,z);	
	if(out_file != "") in.save(out_file.c_str());
    }
    catch(std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

