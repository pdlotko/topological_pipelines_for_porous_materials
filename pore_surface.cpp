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
#include "blow_up_to_mega_cell.h"


using namespace std;



int main( int argc , char** argv )
{
	std::cout << "Obligatory command line parameters: \n";
	std::cout << " 1. (string) Input .cssr sa file name\n";		
	std::cout << " 2. (string) Input .sa file name\n";
	std::cout << " 3. (double) accessible surface area [A^2].\n";
	std::cout << "Optional parameters: \n";
	std::cout << " 4. (double) number of points per surface area. Default: 0.5. \n";
	std::cout << " 5. (string) output .sa file name. Default: out.sa\n";
	std::cout << " 6. (double) Target volume of supercell [A^3]. If not specified, no blow-up is performed.\n";
	if ( argc - 1 < 3 )
	{
		std::cout << "Wrong number of parameters, the program will now terminate.\n";
                return 1;
	}

	std::string cssr_filename = string(argv[1]);
	std::string sa_filename = string(argv[2]);
	double surface_area = atof( argv[3] );

	double how_many_points_per_surface_area = 0.5;	
	if ( argc - 1 >= 4 )
	{
		how_many_points_per_surface_area = atof( argv[4] );
	}

	std::string out_filename = string("out.sa");
	if ( argc - 1 >= 5 )
	{
		out_filename = string( argv[5] );
	}

	double desired_volume = 0;
	if ( argc - 1 >= 6 )
	{
		desired_volume = atof( argv[6] );
	}
	
	std::cout << std::endl;
	std::cout << "Here are the parameters of the program : " << std::endl;
	std::cout << "CSSR Filename : " << cssr_filename << std::endl;
	std::cout << "SA Filename : " << sa_filename << std::endl;
	std::cout << "Accessible surface area [A^2] : " << surface_area << std::endl;
	std::cout << "Output SA Filename : " << out_filename << std::endl;
	std::cout << "Number of points per surface area : " << how_many_points_per_surface_area << std::endl;
	std::cout << "Desired volume [A^3] : " << desired_volume << std::endl;
	std::cout << std::endl;

	//first read the points for thme .sa file
	std::cout << "Reading the points from the .sa file\n";
	vector< vector<double> > points = read_points_from_zeopp_file( sa_filename.c_str() );
	std::cout << points.size() << " points have been read from file " << sa_filename << std::endl;

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

		
//BEGIN READING VECTORS OF UNIT CELL:
	std::vector< double > info_about_unit_cell = read_information_about_unit_cell_from_zeopp_file( cssr_filename.c_str() );
//END READING VECTORS OF UNIT CELL:

//BEGIN BLOWING UP TO A MEGA CELL
        if (desired_volume != 0) 
        {
            std::vector<size_t> how_many_times_in_each_direction(3);
            
            //there are currently two ways we can blow up the megacell. Firstly, we can simpy say how many times we want to blow it up in each direction. Like in the example below:
            //how_many_times_in_each_direction[0] = 2;
            //how_many_times_in_each_direction[1] = 2;
            //how_many_times_in_each_direction[2] = 2;
            //std::pair< std::vector< std::vector<double> > , std::vector< std::vector<double> > > blowed_points = blow_up_to_mega_cell( subsampled_points , info_about_unit_cell , how_many_times_in_each_direction );
            
            //Or we can blow it up till it reach the desired volume like in the example below. 
            std::pair< std::vector< std::vector<double> > , std::vector< std::vector<double> > > blowed_points = blow_up_till_reach_the_desided_volume( subsampled_points , info_about_unit_cell, how_many_times_in_each_direction , desired_volume );
            
            std::vector< std::vector< double > > points_in_mega_cell = blowed_points.first;
            std::vector< std::vector< double > > mega_cells_vector = blowed_points.second;	
            
            std::cerr << "Number of points in mega cell : " << points_in_mega_cell.size() << std::endl;	

            //for test only
            //write_points_to_file_in_vesta_format( points_in_mega_cell , "points_in_mega_cell" );

    //STORING THE RESULTS IN THE FILES.
            
            write_points_to_file( points_in_mega_cell , out_filename.c_str() );
            
            stringstream mega_cell_vector_ss;
            mega_cell_vector_ss << out_filename << ".cell";
            write_mega_cell_vector_to_a_file( mega_cells_vector , mega_cell_vector_ss.str().c_str() );		
        } else {
            //TODO: Write cell vectors also for unit cell
            std::cerr << "No blow-up requested - using unit cell." << std::endl;	
            write_points_to_file( subsampled_points, out_filename.c_str() );
        }

	return 0;
}
