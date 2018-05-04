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
#include "compute_distance_matrix_of_points_in_R3.h"
#include "compute_distance_matrix_relative.h"
//#include "bind_to_ripser.h"



using namespace std;

int main( int argc , char** argv )
{
	std::cout << "The obligatory parameter of the program is the cxxs file. Please provide it WITHOUT extension. \n";	
	if ( argc == 1 )  
	{
		std::cerr << "Wrong number of parameters, the program will now terminate. \n";
		return 1;		
	}
	const char* filename = argv[1];
	std::cout << "The second, optional parameter of the program is the treshold radius for Rips complex computations). \n";
	double threshold = 4.2;
	if ( argc == 3 )
	{
		threshold = atof( argv[2] );
	}
	
	std::cerr << "Here are the parameters of the program : " << std::endl;
	std::cout << "Filename : " << filename << std::endl;
	std::cout << "Treshold : " << threshold << std::endl;	
	
	
	//Reading the points from the megacell:
	stringstream file_with_megacell_points;
	file_with_megacell_points << filename << "_mega_cell";
	vector< vector<double> > points_in_mega_cell = read_points_from_file( file_with_megacell_points.str().c_str() );
	
	std::cout << "We have read : " << points_in_mega_cell.size() << " points from megacell. \n";
	
	//reading megacell vector:
	stringstream megacell_vector_file;
	megacell_vector_file << filename << "_mega_cell_vector";
	std::vector< std::vector< double > > mega_cells_vector = read_mega_cell_vector_from_a_file( megacell_vector_file.str().c_str() );
	
	

//BEGIN COMPUTATIONS OF ALL-TO-ALL DISTANCE MATRIX ON A TORUS
	std::cerr << "Creating all-to-all distance matrix \n";
	std::vector< std::vector< double > > full_dist_matrix = compute_distance_matrix_on_a_torus( points_in_mega_cell ,mega_cells_vector );
	stringstream distance_matrix_file;
	distance_matrix_file << filename << "_distance_matrix";
	write_distance_matrix_to_a_file( full_dist_matrix , distance_matrix_file.str().c_str() );
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
	int result = system ( command.str().c_str() );
	
	//RIPSER WITH SPARSE DISTANCE MATRICES
	//std::cerr << "Running persistence comutations. \n";
	//stringstream command;
	//command << "./ripser_sparse_matrices all-to-all-distances --dim 2 --threshold " <<  threshold << " > " << filename << ".pers";
	//result = system ( command.str().c_str() );
	
//END PERSISTENCE COMPUTATIONS WITH RIPS COMPLEX. 

	return 0;
}
