//Author: Pawel Dlotko
//Last modification: 5 Oct 2016
//Short descrption: a procedure to compute all-to-all distance matrix based on a collection of points and parametrization of the mega cell. 
//For internal use only!

#pragma once

#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>


/**
 * This is a procedure to compute all-to-all distance matrix between points (std::vector< std::vector< double > >& points) on a torus which is obtained by gluing opposite sides
 * of a (skewed) cube spanned by the vectors const std::vector< std::vector< double > >& mega_cells_vector.
**/ 
std::vector< std::vector< double > > compute_distance_matrix_on_a_torus( const std::vector< std::vector< double > >& points , const std::vector< std::vector< double > >& mega_cells_vector )
{
	bool dbg = false;
	if ( dbg )std::cerr << "We have : " << points.size() << " points \n";
	std::vector< std::vector< double > > result;
	result.reserve( points.size() );
	
	euclidena_distance E;
	std::vector<double> second_point(3);
	
	for ( size_t i = 0 ; i != points.size() ; ++i )
	{		
		std::vector< double > v;
		v.reserve( i );
		for ( size_t j = 0 ; j != i ; ++j )		
		{			
			//compute all the distances we need. To be more precise, we have here two points:
			//points[i] and points[j]
			//each of them may be translated along x,y,z axis. To do the computations, we will keep points[i] fixed and move point[j]
			//in general, we can move it to all 27 = 9*3 potisions in all the neighbooring points of the cell. 
			//Therefore at the moment I will do it naivelly. Later, this procedure can be optymized, since points from internal layer just need two computations!
			//TODO - there is optymization to be done here!!!				
			double minimal_distance = std::numeric_limits<double>::max();
			for ( int x = -1 ; x <= 1 ; ++x )
			{
				for ( int y = -1 ; y <= 1 ; ++y )
				{
					for ( int z = -1 ; z <= 1 ; ++z )
					{
						//compute distance between point[i] and 
						//point[j][0]+x*mega_cells_vector[0] , point[j][1]+y*mega_cells_vector[1] , point[j][2]+z*mega_cells_vector[2]. 
						second_point[0] = points[j][0]+x*mega_cells_vector[0][0]+y*mega_cells_vector[1][0]+z*mega_cells_vector[2][0];
						second_point[1] = points[j][1]+x*mega_cells_vector[0][1]+y*mega_cells_vector[1][1]+z*mega_cells_vector[2][1]; 
						second_point[2] = points[j][2]+x*mega_cells_vector[0][2]+y*mega_cells_vector[1][2]+z*mega_cells_vector[2][2]; 
						
						double this_distance = E( points[i] , second_point );
						
						//cerr << "this_distance  " << this_distance << endl;getchar();
						
						if ( minimal_distance > this_distance ) minimal_distance = this_distance;
					}
				}
			} 					
			//and now, the distance between point[i] and point[j] is  minimal_distance
			v.push_back( minimal_distance );		
		}		
		result.push_back(v);
	}
	
	return result;
}//compute_distance_matrix_on_a_torus


/**
 * This is a procedure to compute all-to-all distance matrix between points (std::vector< std::vector< double > >& points). N periodic boundary conditions are added here. 
**/ 
std::vector< std::vector< double > > compute_distance_matrix( const std::vector< std::vector< double > >& points , const std::vector< std::vector< double > >& mega_cells_vector )
{
	std::vector< std::vector< double > > result;
	result.reserve( points.size() );
	
	euclidena_distance E;
	std::vector<double> second_point(3);
	
	for ( size_t i = 0 ; i != points.size() ; ++i )
	{
		std::vector< double > v;
		v.reserve( i );		
		for ( size_t j = 0 ; j != i ; ++j )
		{														
			v.push_back( E( points[i] , points[j] ) );			
		}
		result.push_back(v);
	}
	
	return result;
}//compute_distance_matrix




/**
 * This procedure writes distance matrix to a file in a format sutable for Ripser.
**/ 
void write_distance_matrix_to_a_file( const std::vector< std::vector< double > >& distance_matrix , const char* filename )
{
	ofstream out;
	out.open( filename );	
	for ( size_t i = 0 ; i != distance_matrix.size() ; ++i )
	{
		for ( size_t j = 0 ; j != distance_matrix[i].size() ; ++j )
		{
			out << distance_matrix[i][j] << ",";
		}
		out << std::endl;
	}
	
	out.close();
}//write_distance_matrix_to_a_file



/**
 * This procedure writes the full distance matrix to a file.
**/ 
void write_full_square_distance_matrix_to_a_file( const std::vector< std::vector< double > >& distance_matrix , const char* filename )
{
	ofstream out;
	out.open( filename );	
	for ( size_t i = 0 ; i != distance_matrix.size() ; ++i )
	{		
		for ( size_t j = 0 ; j != distance_matrix.size() ; ++j )
		{
			if ( i < j )
			{
				out << distance_matrix[j][i] << " ";
			}
			else
			{
				if ( i > j )
				{
					out << distance_matrix[i][j] << " ";
				}
				else
				{
					out << "0 ";
				}
			}
		}
		out << std::endl;
	}
	
	out.close();
}//write_distance_matrix_to_a_file
