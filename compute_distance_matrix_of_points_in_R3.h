//Author: Pawel Dlotko
//Last modification: 5 Oct 2016
//Short descrption: A function to compute lower part of a distance matrix given a vector of triples of points. Metric that is used is a template parameter of this function.
//For internal use only!

#pragma once

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;


/**
 * A class defining Euclidean distance in R^3.
**/ 
class euclidena_distance
{
public:
	double operator()( const std::vector< double >& first , const std::vector< double >& second )
	{
		double result = (first[0]-second[0])*(first[0]-second[0]) + (first[1]-second[1])*(first[1]-second[1]) + (first[2]-second[2])*(first[2]-second[2]);
		result = sqrt(result);
		return result;
	}
	double operator()( const double* first , const double* second )
	{
		double result = (first[0]-second[0])*(first[0]-second[0]) + (first[1]-second[1])*(first[1]-second[1]) + (first[2]-second[2])*(first[2]-second[2]);
		result = sqrt(result);
		return result;
	}
};


/**
 * A function to compute lower part of a distance matrix given a vector of triples of points. Metric that is used is a template parameter of this function. 
 * Most typically it will be an Euclidean distance defined above.
**/ 
template <typename F>
std::vector< std::vector< double > > compute_distance_matrix_of_points_in_R3( const std::vector< std::vector< double > >& points )
{
	std::vector< std::vector< double > > result;
	F f;
	for ( size_t i = 0 ; i != points.size() ; ++i )
	{		
		std::vector< double > v;
		v.reserve(i);
		for ( size_t j = 0 ; j != i ; ++j )
		{
			 v.push_back( f( points[i],points[j] ) );
		}
		result.push_back( v );
	}	
	return result;
}//compute_distance_matrix_of_points_in_R3

