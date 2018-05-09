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
//To measure the execution time.
#include <chrono>

//pipeline include;
#include "read_zeopp_file.h"
#include "compute_distance_matrix_of_points_in_R3.h"
#include "compute_distance_matrix_relative.h"
//#include "bind_to_ripser.h"



using namespace std;
using namespace std::chrono;

int main( int argc , char** argv )
{
	std::cout << "The obligatory parameter of the program is the cxxs file (with extension). \n";	
	std::cout << "Please note that a program create_megacell should be run prior to running of this program. It would produce a point cloud from the megacell, which this program will be operating on.\n";

	std::cout << "The second, optional parameter of the program is the treshold radius for Rips complex computations). \n";

	
	if ( argc == 1 )  
	{
		std::cerr << "Wrong number of parameters, the program will now terminate. \n";
		return 1;		
	}
	double threshold = 4.2;
	if ( argc == 3 )
	{
		threshold = atof( argv[2] );
	}
	std::string filename_full_str = string(argv[1]);
	string filename = filename_full_str.substr(0,filename_full_str.size()-5);
	
	std::cerr << "Here are the parameters of the program : " << std::endl;
	std::cout << "Filename : " << filename << std::endl;
	std::cout << "Treshold : " << threshold << std::endl;	
	
	
	//Reading the points from the megacell:
	stringstream file_with_megacell_points;
	file_with_megacell_points << filename << "_mega_cell";
	high_resolution_clock::time_point begin = high_resolution_clock::now();
	vector< vector<double> > points_in_mega_cell = read_points_from_file( file_with_megacell_points.str().c_str() );
	std::cout << "We have read : " << points_in_mega_cell.size() << " points from megacell. \n";
	
	//reading megacell vector:
	stringstream megacell_vector_file;
	megacell_vector_file << filename << "_mega_cell_vector";
	std::vector< std::vector< double > > mega_cells_vector = read_mega_cell_vector_from_a_file( megacell_vector_file.str().c_str() );
	
	

//BEGIN COMPUTATIONS OF ALL-TO-ALL DISTANCE MATRIX ON A TORUS
	std::cerr << "Creating all-to-all distance matrix \n";
		
	std::vector< std::vector< double > > full_dist_matrix = compute_distance_matrix_on_a_torus( points_in_mega_cell ,mega_cells_vector );
	high_resolution_clock::time_point all_to_all_distance_matrix_is_computed = high_resolution_clock::now();
	
	stringstream distance_matrix_file;
	distance_matrix_file << filename << "_distance_matrix";
	write_distance_matrix_to_a_file( full_dist_matrix , distance_matrix_file.str().c_str() );
	high_resolution_clock::time_point points_are_written_to_file = high_resolution_clock::now();
//END COMPUTATIONS OF ALL-TO-ALL DISTANCE MATRIX ON A TORUS


//BEGIN PERSISTENCE WITH RIPS COMPUTATIONS (with ripser. Right now, at the beginning, I am calling external system command to do the computations. Later, if needed, I will integrate it into this C++ code.)	
	//RIPSER
	//std::cerr << "Running persistence comutations. \n";
	//stringstream command;
	//command << "./ripser all-to-all-distances --dim 2 --threshold " <<  threshold << " > " << filename << ".pers";
	//result = system ( command.str().c_str() );	
	
	//GUDHI:
	stringstream command;
	command << "./persistence_from_distance_matrix -d 3 -r " << threshold << " -o " << filename << ".pers "  << distance_matrix_file.str();
	int res = system ( command.str().c_str() );
	if ( res != 0 )std::cout << "Some error have occured. \n";
	
	high_resolution_clock::time_point peristence_has_been_computed = high_resolution_clock::now();
	
	//RIPSER WITH SPARSE DISTANCE MATRICES
	//std::cerr << "Running persistence comutations. \n";
	//stringstream command;
	//command << "./ripser_sparse_matrices all-to-all-distances --dim 2 --threshold " <<  threshold << " > " << filename << ".pers";
	//result = system ( command.str().c_str() );
	
//END PERSISTENCE COMPUTATIONS WITH RIPS COMPLEX. 

	auto creation_of_matrix = duration_cast<seconds>( all_to_all_distance_matrix_is_computed - begin ).count();
	auto writing_matrix_to_file = duration_cast<seconds>( points_are_written_to_file - all_to_all_distance_matrix_is_computed ).count();
	auto persistence_computations = duration_cast<seconds>( peristence_has_been_computed - points_are_written_to_file ).count();
	std::cout << "Here are some data about the time execution of the program : \n";
	std::cout << "Creation of a distance matrix: " << creation_of_matrix << std::endl;
	std::cout << "Writing a distance matrix to a file: " << writing_matrix_to_file << std::endl;
	std::cout << "Persistence computations: " << persistence_computations << std::endl;

	return 0;
}
