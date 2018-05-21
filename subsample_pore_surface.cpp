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
#include "compute_distance_matrix_of_points_in_R3.h"


using namespace std;



int main( int argc , char** argv )
{
	std::cout << "Obligatory command line parameters: \n";
	std::cout << "(1) Input .sa file name (string)\n";		
	std::cout << "(2) accessible surface area (double).\n";
	std::cout << "Optional parameters: \n";
	std::cout << "(3) output .sa file name (string). Default: out.sa. \n";
	std::cout << "(4) number of points per surface area (double). Default: 0.5. \n";
	std::cout << "(5) Subsample procedure to be used. The options are: 1 = Random subsample, 2 = max_min subsample and 3 = grid based subsample. Default: 2. \n";
	if ( argc < 3 )
	{
		std::cout << "Wrong number of parameters, the program will now terminate.\n";
                return 1;
	}
	std::string sa_filename = string(argv[1]);
	double surface_area = atof( argv[2] );

	std::string out_filename = string("out.sa");
	if ( argc > 2 )
	{
		out_filename = string( argv[3] );
	}

	double how_many_points_per_surface_area = 0.5;	
	if ( argc > 3 )
	{
		how_many_points_per_surface_area = atof( argv[4] );
	}
	
	int which_subsample_method = 2;
	if ( argc > 4 )
	{
		which_subsample_method = atoi( argv[5] );
	}

	std::cout << "Here are the parameters of the program : " << std::endl;
	std::cout << "SA Filename : " << sa_filename << std::endl;
	std::cout << "Accessible surface area : " << surface_area << std::endl;
	std::cout << "Output SA Filename : " << out_filename << std::endl;
	std::cout << "Number of points per surface area : " << how_many_points_per_surface_area << std::endl;
	std::cout << "Which subsample method : " << which_subsample_method << std::endl;

	//first read the points for thme .sa file
	std::cout << "Reading the points from the .sa file\n";
	vector< vector<double> > points = read_points_from_zeopp_file( sa_filename.c_str() );
	std::cout << points.size() << " points have been read from file. \n";


	size_t size_of_subsample = (size_t)surface_area*how_many_points_per_surface_area;
	std::cout << "Number of points to subsample : " << size_of_subsample << std::endl;
	
	





    //BEGIN FINDING SUBSAMPLE. Here we have a few ways of doing it.
	////example of random subsample:
	
	vector< vector<double> > subsampled_points;
	if ( which_subsample_method == 1 )
	{
		//random subsample
		subsampled_points = random_subsample( points , size_of_subsample );
	}
	else
	{
		if ( which_subsample_method == 2 )
		{
			//max_min subsample
			//Less memory efficient, but sometimes faster version.
			//std::vector< std::vector<double> > subsampled_points = max_min_based_subsample_procedure( points , size_of_subsample );
			//More memory efficient, but sometimes slower version.
			subsampled_points =  max_min_subset_certain_size< std::vector<double>, euclidena_distance >( points , size_of_subsample  );
		}
		else
		{
			if ( which_subsample_method == 3 )
			{
				//grid based subsample. Over here we assume that the size of the cube is to be at least 1 A^3.
				std::vector< std::pair< double,double > > ranges = find_ranges_of_point_cloud( points );
				double weight = 1;
				size_t number_of_cubes_direction_x = (size_t)(weight*(ranges[0].second - ranges[0].first));
				size_t number_of_cubes_direction_y = (size_t)(weight*(ranges[1].second - ranges[1].first));
				size_t number_of_cubes_direction_z = (size_t)(weight*(ranges[2].second - ranges[2].first)); 
				size_t how_many_points_to_sample_from_cube = 1;
				std::cout << "Here are the sizes of the cubical grid usied for subsampling : \n In direstion x : " << number_of_cubes_direction_x << "\n In direction y : " << number_of_cubes_direction_y << "\n In direction z : " << number_of_cubes_direction_z << std::endl;			
				subsampled_points = subsample_by_using_grid( points , number_of_cubes_direction_x , number_of_cubes_direction_y , number_of_cubes_direction_z , how_many_points_to_sample_from_cube  );
			}
			else
			{
				std::cerr << "UNKNOWN SUBSAMPLE METHOD, THE PROGRAM WILL NOW TERMINATE. \n";
				return 1;				
			}
		}
	}
	
	std::cout << "Numeber of points after subsampling : " << subsampled_points.size() << std::endl;


	//For test only
	write_points_to_file_in_vesta_format( subsampled_points , "subsampled_points.xyz" );

	write_points_to_file( subsampled_points , out_filename.c_str() );

	return 0;
}
