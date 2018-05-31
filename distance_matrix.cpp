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
        std::cout << "Obligatory command line parameters: \n";
        std::cout << " 1. (string) Input .vsa file name\n";
        std::cout << " 2. (string) Input .cell file name\n";
        std::cout << "Optional parameters: \n";
        std::cout << " 3. (string) Output .matrix file name. Default: distance.matrix\n";

        if ( argc - 1 < 2 or argc -1 > 3 )
        {
            std::cout << "Wrong number of parameters, the program will now terminate.\n";
            return 1;
        }

	std::string vsa_filename = string(argv[1]);
	std::string cell_filename = string(argv[2]);

	std::string matrix_filename = "distance.matrix";
        if ( argc - 1 == 3 )
        {
            matrix_filename = string(argv[3]);
        }
	
	std::cout << "Here are the parameters of the program : " << std::endl;
	std::cout << "VSA filename : " << vsa_filename << std::endl;
	std::cout << "CELL filename : " << cell_filename << std::endl;
	std::cout << "Output filename : " << matrix_filename << std::endl;
	
	//Reading the points from the megacell:
	high_resolution_clock::time_point begin = high_resolution_clock::now();
	vector< vector<double> > points_in_mega_cell = read_points_from_file( vsa_filename.c_str() );
	std::cout << "We have read : " << points_in_mega_cell.size() << " points from megacell. \n";
	
	//reading megacell vector:
	std::vector< std::vector< double > > mega_cells_vector = read_mega_cell_vector_from_a_file( cell_filename.c_str() );
	
//BEGIN COMPUTATIONS OF ALL-TO-ALL DISTANCE MATRIX ON A TORUS
	std::cout << "Creating all-to-all distance matrix \n";
		
	std::vector< std::vector< double > > full_dist_matrix = compute_distance_matrix_on_a_torus( points_in_mega_cell ,mega_cells_vector );
	high_resolution_clock::time_point all_to_all_distance_matrix_is_computed = high_resolution_clock::now();
	
	write_distance_matrix_to_a_file( full_dist_matrix , matrix_filename.c_str() );
	high_resolution_clock::time_point points_are_written_to_file = high_resolution_clock::now();
//END COMPUTATIONS OF ALL-TO-ALL DISTANCE MATRIX ON A TORUS


	auto creation_of_matrix = duration_cast<seconds>( all_to_all_distance_matrix_is_computed - begin ).count();
	auto writing_matrix_to_file = duration_cast<seconds>( points_are_written_to_file - all_to_all_distance_matrix_is_computed ).count();
	std::cout << "Here are some data about the time execution of the program : \n";
	std::cout << "Creation of a distance matrix: " << creation_of_matrix << std::endl;
	std::cout << "Writing a distance matrix to a file: " << writing_matrix_to_file << std::endl;

	return 0;
}
