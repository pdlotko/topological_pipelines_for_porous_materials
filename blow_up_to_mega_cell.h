//Author: Pawel Dlotko 
//Last modification: 5 Oct 2016
//Short descrption: The procedure presented in this file take a collection of points form a unit cell, the parameters of a unit cell, and information how many times in each direction it should replicate the unit cell.
//It provide as an output the set of points from the mega cell.
//For internal use only!

#pragma once

#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "compute_distance_matrix_of_points_in_R3.h"



/**
 * This is a code to compute translation vector (or a vector the convex hull of which is a considered cell). 
 * The input parameter is the parameter of unit cell computed by read_information_about_unit_cell_from_zeopp_file function
**/ 
std::vector< std::vector< double > > compute_translation_vector( const std::vector< double >& parameters_of_unit_cell ) 
{
	bool dbg = false;
	//first we need to transalte the strange zeo++ based const std::vector< double >& parameters_of_unit_cell into nice vectors in R^3:
	std::vector< double > vector_1(3);
	std::vector< double > vector_2(3);
	std::vector< double > vector_3(3);
	
	//to compute those vectors I am using a formula from https://en.wikipedia.org/wiki/Fractional_coordinates
	//For that in below I am tranlatin the data from parameters_of_unit_cell into a format used in wikipedia:
	double a = parameters_of_unit_cell[0];
	double b = parameters_of_unit_cell[1];
	double c = parameters_of_unit_cell[2];
	
	double alpha = parameters_of_unit_cell[3]/360.0 * 2*3.141592;
	double beta = parameters_of_unit_cell[4]/360.0 * 2*3.141592;
	double gamma = parameters_of_unit_cell[5]/360.0 * 2*3.141592; 
	
	vector_1[0] = a;
	vector_1[1] = 0;
	vector_1[2] = 0;
	
	vector_2[0] = b*cos( gamma );
	vector_2[1] = b*sin( gamma );
	vector_2[2] = 0;
	
	vector_3[0] = c*cos( beta );
	vector_3[1] = c*( (cos(alpha)-cos(beta)*cos(gamma))/(sin(gamma)) );
	vector_3[2] = c*( sqrt( 1-cos(alpha)*cos(alpha) -cos(beta)*cos(beta) - cos(gamma)*cos(gamma) + 2*cos(alpha)*cos(beta)*cos(gamma) )/sin(gamma) );
	
	std::vector< std::vector< double > > translation_vector;
	translation_vector.push_back( vector_1 );
	translation_vector.push_back( vector_2 );
	translation_vector.push_back( vector_3 );
	
	//if ( dbg )
	{
		std::cerr << "Here is the translation vector \n";
		std::cerr << "v0 : " << vector_1[0] << " " << vector_1[1] << " " << vector_1[2] << std::endl;
		std::cerr << "v1 : " << vector_2[0] << " " << vector_2[1] << " " << vector_2[2] << std::endl;
		std::cerr << "v2 : " << vector_3[0] << " " << vector_3[1] << " " << vector_3[2] << std::endl;
	}
	return translation_vector;
}

/**
 * The procedure presented in this file take a collection of points form a unit cell, the parameters of a unit cell (length in the first, second and third direction
 * angle between first and second vector, angle between first and third vector, angle between second and third vector, and information how many times in each direction 
 * it should replicate the unit cell (as a vector of a size 3).
 * It provide as an output a pair: 
 * a vector< vector<double> > representing  points from the mega cell and
 * vector of vector of double, representing ector representation of the mega cell.
**/
std::pair< std::vector< std::vector<double> > , std::vector< std::vector<double> > > blow_up_to_mega_cell( const std::vector< std::vector<double> >& points , const std::vector< double >& parameters_of_unit_cell , 
														 std::vector<size_t> how_many_times_in_each_direction )
{
	bool dbg = false;
	
	if ( dbg )
	{
		std::cerr << "Enterign the blow_up_to_mega_cell procedure \n";
	}
	
	std::vector< std::vector< double > > translation_vector = compute_translation_vector( parameters_of_unit_cell );
	std::vector< std::vector<double> > new_points(points);
	
	cerr << "new_points.size() : " << new_points.size() << endl;
	
	//and now we can do our translations:
	for ( size_t dim = 0 ; dim != 3 ; ++dim )
	{
		if ( dbg )
		{
			std::cerr << "Doing translation in the dimension : " << dim << std::endl;
		}
		std::vector< std::vector<double> > points_after_translation_in_this_direction;
		//we do translation in the direction of dimension dim (0 = x, 1 = y, 2 = z).
		for ( size_t how_many_times = 1; how_many_times <= how_many_times_in_each_direction[dim] ; ++how_many_times )
		{
			//for each point:
			for ( size_t pt = 0 ; pt != new_points.size() ; ++pt )
			{
				//translate it by how_many_times*translation_vector[dim]
				std::vector<double> v(3);
				v[0] = new_points[pt][0] + translation_vector[dim][0]*how_many_times;
				v[1] = new_points[pt][1] + translation_vector[dim][1]*how_many_times;
				v[2] = new_points[pt][2] + translation_vector[dim][2]*how_many_times;
				points_after_translation_in_this_direction.push_back( v );
			}
		}
		new_points.swap( points_after_translation_in_this_direction );
		if ( dbg )
		{
			std::cerr << "new_points.size() : " << new_points.size() << std::endl;
		}
	}
	
	//and we also need to rescale the translation_vector so that it fit the mega cell:
	translation_vector[0][0] *= how_many_times_in_each_direction[0];
	translation_vector[0][1] *= how_many_times_in_each_direction[0];
	translation_vector[0][2] *= how_many_times_in_each_direction[0];
	
	translation_vector[1][0] *= how_many_times_in_each_direction[1];
	translation_vector[1][1] *= how_many_times_in_each_direction[1];
	translation_vector[1][2] *= how_many_times_in_each_direction[1];
	
	translation_vector[2][0] *= how_many_times_in_each_direction[2];
	translation_vector[2][1] *= how_many_times_in_each_direction[2];
	translation_vector[2][2] *= how_many_times_in_each_direction[2];
	
	return std::make_pair( new_points , translation_vector );
}//blow_up_to_mega_cell



std::pair< std::vector< std::vector<double> > , std::vector< std::vector<double> > > 
blow_up_till_reach_the_desided_volume( const std::vector< std::vector<double> >& points , 
									   const std::vector< double >& parameters_of_unit_cell , 
									   std::vector<size_t>& how_many_times_in_each_direction,
									   double desired_volume = 40000 )
{
	//A few semi--theretical studies. If we replicate a unit cells at each side of the constructed megacell, then the volumes of the following steps will scale like:
	//1, 8, 27, ...
	//so like
	//1^3*volume_of_unit_cell, 2^3*volume_of_unit_cell, 3^3*volume_of_unit_cell, 4^3*volume_of_unit_cell etc.
	//So the equation we need to solve is: volume_of_unit_cell*n^3 = desired_volume and round up to the nearest integer.
	//To use this formula, we need to know the volume of the unit cell. This is what we are computing in below:
	
	std::vector< std::vector< double > > translation_vector = compute_translation_vector( parameters_of_unit_cell );
	double length_of_vector_in_x_direction = sqrt( translation_vector[0][0]*translation_vector[0][0] + translation_vector[0][1]*translation_vector[0][1] + translation_vector[0][2]*translation_vector[0][2] );
	double length_of_vector_in_y_direction = sqrt( translation_vector[1][0]*translation_vector[1][0] + translation_vector[1][1]*translation_vector[1][1] + translation_vector[1][2]*translation_vector[1][2] );
	double length_of_vector_in_z_direction = sqrt( translation_vector[2][0]*translation_vector[2][0] + translation_vector[2][1]*translation_vector[2][1] + translation_vector[2][2]*translation_vector[2][2] );
	
	std::cerr << "length_of_vector_in_x_direction : " << length_of_vector_in_x_direction << std::endl;
	std::cerr << "length_of_vector_in_y_direction : " << length_of_vector_in_y_direction << std::endl;
	std::cerr << "length_of_vector_in_z_direction : " << length_of_vector_in_z_direction << std::endl;
	
	double volume_of_unit_cell = length_of_vector_in_x_direction*length_of_vector_in_y_direction*length_of_vector_in_z_direction;
	
	std::cerr << "volume_of_unit_cell : " << volume_of_unit_cell << endl;
	
	int number_of_times_we_should_blow_up_the_cell = (int)pow((desired_volume/volume_of_unit_cell),0.333333);
	++number_of_times_we_should_blow_up_the_cell;//we increment this counter, since if it is 1, then we need to add one extra layer 
	 
	std::cout << "To obtain the desired volume of : " << desired_volume << " we should blow up our unit cell of a volume : " << volume_of_unit_cell << " " << number_of_times_we_should_blow_up_the_cell << " times \n";
	
	how_many_times_in_each_direction.clear();
	how_many_times_in_each_direction.push_back( number_of_times_we_should_blow_up_the_cell );
	how_many_times_in_each_direction.push_back( number_of_times_we_should_blow_up_the_cell );
	how_many_times_in_each_direction.push_back( number_of_times_we_should_blow_up_the_cell );
	
	return blow_up_to_mega_cell( points , parameters_of_unit_cell , how_many_times_in_each_direction );
}//blow_up_till_reach_the_desided_volume


