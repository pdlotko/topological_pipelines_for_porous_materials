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
	std::cout << "The obligatory parameter of the program is: \n";
	std::cout << "(1) The name of cxxs file (string) (with extension). \n";		
	std::cout << "(2) The accesibe surface area (double). \n";
	std::cout << "The optional parameters of this program are: \n";
	std::cout << "(3) (double) - number of points per surface area. Set to 0.5 by default. \n";
	//std::cout << "(3) (double) - radius of the probe. Set to 1.2 by default. \n";
	//std::cout << "(4) (integer) - initial number of points sampled using zeo++. 2000 by default. \n";
	double how_many_points_per_surface_area = 0.5;	
	//double radius_of_a_probe = 1.2;
	//size_t initial_number_of_points_sampled_from_zeopp = 2000;	
	if ( argc < 3 )
	{
		std::cout << "Wrong number of parameters, the program will now terminate.\n";
	}
	std::string filename_full_str = string(argv[1]);
	string filename = filename_full_str.substr(0,filename_full_str.size()-5);
	double surface_area = atof( argv[2] );
	if ( argc > 3 )
	{
		how_many_points_per_surface_area = atof( argv[3] );
	}
	//if ( argc >= 4 )
	//{
	//	radius_of_a_probe = atof( argv[3] );
	//}
	//if ( argc >= 5 )
	//{
	//	initial_number_of_points_sampled_from_zeopp = atoi( argv[4] );
	//}	

	std::cout << "Here are the parameters of the program : " << std::endl;
	std::cout << "Filename : " << filename << std::endl;
	std::cout << "Accessable surface area : " << surface_area << std::endl;
	std::cout << "Number of points per surface area : " << how_many_points_per_surface_area << std::endl;
	//std::cout << "Radius of the probe : " << radius_of_a_probe << std::endl;
	//std::cout << "Number of points inially sampled from zeo++ : " << initial_number_of_points_sampled_from_zeopp << std::endl;
	

	//Note that we require that the network, which is executable from zeo++ is in the same folder in which this code is executed. 

	//As discussed with Leo, I am not calling the zeopp code, but assuming that the file with the points is there 
	//stringstream command_zeopp;
	//command_zeopp << "./network -vsa " << radius_of_a_probe << " " << radius_of_a_probe << " " << initial_number_of_points_sampled_from_zeopp << " " << argv[1] << ".sa " << argv[1] << ".cssr";
	//int result = system ( command_zeopp.str().c_str() );

	//first read the points for thme .sa file
	stringstream ss;
	ss << filename << ".sa";
	std::cout << "Reading the points from the .sa file\n";
	vector< vector<double> > points = read_points_from_zeopp_file( ss.str().c_str() );
	std::cout << points.size() << " points have been read from file. \n";

	//write_points_to_file_in_vesta_format( points , "points_before_subsampling" );//TODO!!!!


	//GETTING THE ACCESSABLE SURFACE AREA:
	//stringstream command_zeopp_accesible_surface_area;
	/// ./network -ha -sa 1.2  1.2 2000  h8325989_asa.sa h8325989.cssr
	//command_zeopp_accesible_surface_area << "./network -ha -sa " << radius_of_a_probe << " " << radius_of_a_probe << " 2000 " << argv[1] << "_asa.sa " << argv[1] << ".cssr";
	//result = system ( command_zeopp_accesible_surface_area.str().c_str() );
	//stringstream accesible_surface_area_file;
	//accesible_surface_area_file << argv[1] << "_asa.sa";
	//double surface_area = get_accessible_surface_area_from_sa_file(  accesible_surface_area_file.str().c_str() );
	size_t size_of_subsample = (size_t)surface_area*how_many_points_per_surface_area;
	std::cout << "Number of points to subsample : " << size_of_subsample << std::endl;
	
	//for test only:
	//write_points_to_file_in_vesta_format( points , "points_before_subsample" );
	
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

	stringstream ss1;
	ss1 << filename << "_subsampled";
	write_points_to_file( subsampled_points , ss1.str().c_str() );

	return 0;
}
