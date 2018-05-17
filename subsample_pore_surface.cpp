//Author: Pawel Dlotko
//Last modification: 2 May 2018
//Short descrption: Exampe of a procedure to call zeo++, use it to sample 
//points from pore surface and subsample them.
//For internal use only!

//C++ include. 
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

//pipeline include;
#include "read_zeopp_file.h"
#include "random_subsampling_of_a_list_of_points.h"
#include "find_epsilon_net.h"


using namespace std;



int main( int argc , char** argv )
{
	std::cout << "Obligatory command line parameters: \n";
	std::cout << "(1) Input .sa file name (string)\n";		
	std::cout << "(2) accessible surface area (double).\n";
	std::cout << "Optional parameters: \n";
	std::cout << "(3) output .sa file name (string). Default: out.sa. \n";
	std::cout << "(4) number of points per surface area (double). Default: 0.5. \n";
	if ( argc < 3 )
	{
		std::cout << "Wrong number of parameters, the program will now terminate.\n";
                return 1;
	}
	std::string sa_filename = string(argv[1]);
	double surface_area = atof( argv[2] );

	std::string out_filename = string("out.sa");
	if ( argc -1 > 3 )
	{
		out_filename = string( argv[3] );
	}

	double how_many_points_per_surface_area = 0.5;	
	if ( argc-1 > 4 )
	{
		how_many_points_per_surface_area = atof( argv[4] );
	}

	std::cout << "Here are the parameters of the program : " << std::endl;
	std::cout << "SA Filename : " << sa_filename << std::endl;
	std::cout << "Accessible surface area : " << surface_area << std::endl;
	std::cout << "Output SA Filename : " << out_filename << std::endl;
	std::cout << "Number of points per surface area : " << how_many_points_per_surface_area << std::endl;

	//first read the points for thme .sa file
	std::cout << "Reading the points from the .sa file\n";
	vector< vector<double> > points = read_points_from_zeopp_file( sa_filename.c_str() );
	std::cout << points.size() << " points have been read from file. \n";

	//write_points_to_file_in_vesta_format( points , "points_before_subsampling" );//TODO!!!!

	size_t size_of_subsample = (size_t)surface_area*how_many_points_per_surface_area;
	std::cout << "Number of points to subsample : " << size_of_subsample << std::endl;
	
	//NOTE THAT BELOW WE HAVE DIFFERENT SUBSAMPLING METHODS THAT OUGHT TO BE TESTED!!!!


        //BEGIN FINDING SUBSAMPLE. Here we have a few ways of doing it.
	////example of random subsample:
	//vector< vector<double> > subsampled_points = random_subsample( points , size_of_subsample );

	//subsamplng points via min-max algorithm using distance matrix. :

	std::vector< std::vector<double> > subsampled_points = max_min_based_subsample_procedure( points , size_of_subsample );
	points = subsampled_points;

	//subsampling points via epsilon--net algorithm:
	//std::vector< std::vector<double> > subsampled_points = epsilon_net_based_subsample_procedure( points , diameter_of_epsilon_net );

	//subsampling viam max_min algorithm without proucing an all-to-all distance matrix. Should be used for large point clouds:
	//std::vector< vector<double> > subsampled_points =  max_min_subset_certain_size< vector<double> , euclidena_distance >( points , size_of_subsample );

	std::cout << "Numeber of points after subsampling : " << subsampled_points.size() << std::endl;


	//For test only
	//write_points_to_file_in_vesta_format( subsampled_points , "subsampled_points" );

	write_points_to_file( subsampled_points , out_filename.c_str() );

	return 0;
}
