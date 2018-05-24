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
	std::cout << " 1. (string) Input .cssr file name\n";		
	std::cout << " 2. (string) Input .vsa file name\n";
	std::cout << " 3. (double) accessible surface area [A^2].\n";
	std::cout << "Optional parameters: \n";
	std::cout << " 4. (double) density of points on surface area [1/A^2]. Default: 0.5. \n";
	std::cout << " 5. (string) output .vsa file name. Default: out.vsa\n";
	std::cout << " 6. (double) Target volume of supercell [A^3]. If not specified, no blow-up is performed.\n";
	std::cout << " 7. (optional) Subsample procedure to be used. The options are: 1 = Random subsample, 2 = max_min subsample and 3 = grid based subsample (*), 4 = greedy based epsilon net  (*) (with epsilon set to 1 A). Default: 2. \n Note that the algorithms marked with (*) do not guarantee the size of the output point cloud. \n";
	std::cout << " 8. (optional) 0-1 parameter. If 0, the uniform blow up of the unit cell will be performed. In the other case, the non uniform blow up will be made (so that the resulting mega cell is as close to a cube as possible). 1 by default. \n";
	if ( argc - 1 < 3 )
	{
		std::cout << "Wrong number of parameters, the program will now terminate.\n";
                return 1;
	}

	std::string cssr_filename = string(argv[1]);
	std::string vsa_filename = string(argv[2]);
	double surface_area = atof( argv[3] );

	double how_many_points_per_surface_area = 0.5;	
	if ( argc  > 4 )
	{
		how_many_points_per_surface_area = atof( argv[4] );
	}

	std::string out_filename = string("out.vsa");
	if ( argc > 5 )
	{
		out_filename = string( argv[5] );
	}

	double desired_volume = 0;
	if ( argc > 6 )
	{
		desired_volume = atof( argv[6] );
	}
	
	int which_subsample_method = 2;
	if ( argc > 7 )
	{
		which_subsample_method = atoi( argv[7] );
	}
	
	int blow_up_scheme = 1;
	if ( argc > 8 )
	{
		blow_up_scheme = atoi( argv[8] );
	}
	
	std::cout << std::endl;
	std::cout << "Here are the parameters of the program : " << std::endl;
	std::cout << "CSSR Filename : " << cssr_filename << std::endl;
	std::cout << "VSA Filename : " << vsa_filename << std::endl;
	std::cout << "Accessible surface area [A^2] : " << surface_area << std::endl;
	std::cout << "Output SA Filename : " << out_filename << std::endl;
	std::cout << "Density of points on surface area [1/A^2] : " << how_many_points_per_surface_area << std::endl;
	std::cout << "Desired volume [A^3] : " << desired_volume << std::endl;
	std::cout << std::endl;

	//first read the points for thme .vsa file
	std::cout << "Reading the points from the .vsa file\n";
	vector< vector<double> > points = read_points_from_zeopp_file( vsa_filename.c_str() );
	std::cout << points.size() << " points have been read from file " << vsa_filename << std::endl;

	//for tests only.
	//write_points_to_file_in_vesta_format( points , "points_before_subsampling" );//TODO!!!!

	size_t size_of_subsample = (size_t)surface_area*how_many_points_per_surface_area;
	std::cout << "Number of points to subsample : " << size_of_subsample << std::endl;
	
	
	
	vector< vector<double> > subsampled_points;
	if ( which_subsample_method == 1 )
	{
		//random subsample
		subsampled_points = random_subsample( points , size_of_subsample );
	}
	else if ( which_subsample_method == 2 )
	{
		//max_min subsample
		//Less memory efficient, but sometimes faster version.
		//std::vector< std::vector<double> > subsampled_points = max_min_based_subsample_procedure( points , size_of_subsample );
		//More memory efficient, but sometimes slower version.
		subsampled_points =  max_min_subset_certain_size< std::vector<double>, euclidena_distance >( points , size_of_subsample  );
	}
	else if ( which_subsample_method == 3 )
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
	else if ( which_subsample_method == 4 )  
	{
		double epsilon = 2;
		subsampled_points = build_epsilon_net_gready_algorithm< std::vector<double>, euclidena_distance >( points , epsilon );
	}
	else
	{
		std::cerr << "UNKNOWN SUBSAMPLE METHOD, THE PROGRAM WILL NOW TERMINATE. \n";
		return 1;				
	}
	
	

	std::cout << "Number of points after subsampling : " << subsampled_points.size() << std::endl;


	//For test only
	//write_points_to_file_in_vesta_format( subsampled_points , "subsampled_points" );

		
//BEGIN READING VECTORS OF UNIT CELL:
	std::vector< double > info_about_unit_cell = read_information_about_unit_cell_from_zeopp_file( cssr_filename.c_str() );
//END READING VECTORS OF UNIT CELL:

//BEGIN BLOWING UP TO A MEGA CELL
        if (desired_volume != 0) 
        {
            std::vector<size_t> how_many_times_in_each_direction(3);          
            //Or we can blow it up till it reach the desired volume like in the example below. 
           
            std::pair< std::vector< std::vector<double> > , std::vector< std::vector<double> > > blowed_points;
			if ( blow_up_scheme == 0 )
			{
				//This procedure blows up the same number fo times in each direction. If we start from not isotropic unit cell, we will end up with a non isotrpic mega cell.
				blowed_points = blow_up_till_reach_the_desided_volume( subsampled_points , info_about_unit_cell, how_many_times_in_each_direction , desired_volume );
			}
			else
			{
				//This procedure blows up the unit cell so that the mega cell desired volume is approximated as closely as possible and in addition the mega cell is as close to isotropic cube as possible
				blowed_points = blow_up_till_reach_the_desided_volume_and_produce_isotropic_mega_cell( subsampled_points , info_about_unit_cell, how_many_times_in_each_direction , desired_volume );
			}
            
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
            std::cout << "No blow-up requested - using unit cell." << std::endl;	
            write_points_to_file( subsampled_points, out_filename.c_str() );
        }

	return 0;
}
