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

//pipeline include;
#include "read_zeopp_file.h"
#include "blow_up_to_mega_cell.h"
//#include "bind_to_ripser.h"



using namespace std;


int main( int argc , char** argv )
{
	std::cout << "The obligatory parameter of the program is the cxxs file (with extension). \n";	
	std::cout << "The second, optional parameter is a desired volume (set to 40000 A^3 by defailt). \n";	
	std::cout << "The last, third optional parameter is a 0-1 parameter. If 0, the uniform blow up of the unit cell will be performed. In the other case, the non uniform blow up will be made (so that the resulting mega cell is as close to a cube as possible). 1 by default. \n";
	std::cout << "Please note that prior to execution of this procedure, one should run the subsample_pore_surface, which will create a file with a point cloud sampled from the pores of the considered structure.\n";
	if ( argc == 1 )  
	{
		std::cerr << "Wrong number of parameters, the program will now terminate. \n";
		return 1;		
	}
	std::string filename_full_str = string(argv[1]);
	string filename = filename_full_str.substr(0,filename_full_str.size()-5);
	std::cout << "The second, optional parameter of the program is the target volume of the blowed up configuration (by default set up to 40000 A^3). \n";
	double desired_volume = 40000;
	if ( argc >= 3 )
	{
		desired_volume = atof( argv[2] );
	}
	int blow_up_scheme = 1;
	if ( argc == 4 )
	{
		blow_up_scheme = atoi( argv[3] );
	}	
	
	std::cerr << "Here are the parameters of the program : " << std::endl;
	std::cout << "Filename : " << filename << std::endl;
	std::cout << "Desired volume : " << desired_volume << std::endl;
		
//BEGIN READING VECTORS OF UNIT CELL:
	stringstream ss1;
	ss1 << filename << ".cssr";
	std::vector< double > info_about_unit_cell = read_information_about_unit_cell_from_zeopp_file( ss1.str().c_str() );
//END READING VECTORS OF UNIT CELL:

//Now we read from the file the sampled points that have been stored there by using the procedure in the file create_and_subsample_pore_surface.cpp  
	stringstream file_with_points;
	file_with_points << filename << "_subsampled";
    vector< vector<double> > subsampled_points = read_points_from_file( file_with_points.str().c_str() );
    std::cout << subsampled_points.size() << " points have been read from the : " << file_with_points.str() << " file. \n";
	
//BEGIN BLOWING UP TO A MEGA CELL
	std::vector<size_t> how_many_times_in_each_direction(3);
	
	//there are currently two ways we can blow up the megacell. Firstly, we can simpy say how many times we want to blow it up in each direction. Like in the example below:
	//how_many_times_in_each_direction[0] = 2;
	//how_many_times_in_each_direction[1] = 2;
	//how_many_times_in_each_direction[2] = 2;
	//std::pair< std::vector< std::vector<double> > , std::vector< std::vector<double> > > blowed_points = blow_up_to_mega_cell( subsampled_points , info_about_unit_cell , how_many_times_in_each_direction );
	
	std::pair< std::vector< std::vector<double> > , std::vector< std::vector<double> > > blowed_points;
	//Or we can blow it up till it reach the desired volume like in the example below. 
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
	write_points_to_file_in_vesta_format( points_in_mega_cell , "points_in_mega_cell.xyz" );

//STORING THE RESULTS IN THE FILES.
	
	stringstream points_in_mega_cell_ss;
	points_in_mega_cell_ss << filename << "_mega_cell";
	write_points_to_file( points_in_mega_cell , points_in_mega_cell_ss.str().c_str() );
	
	//for ( size_t i = 0 ; i != mega_cells_vector.size() ; ++i ) 
	//{
	//	for ( size_t j = 0 ; j != mega_cells_vector[i].size() ; ++j )
	//	{
	//		cerr << mega_cells_vector[i][j] << " ";
	//	}
	//	cerr << endl;
	//}
			
	stringstream mega_cell_vector_ss;
	mega_cell_vector_ss << filename << "_mega_cell_vector";
	write_mega_cell_vector_to_a_file( mega_cells_vector , mega_cell_vector_ss.str().c_str() );		
		
	return 0;
}
