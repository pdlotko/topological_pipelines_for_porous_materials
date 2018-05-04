//Author: Pawel Dlotko
//Last modification: 5 Oct 2016
//Short descrption: A function randomy subsample a collection of points in R^3
//For internal use only!

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


/**
 * This procedure produce a random subsample of a vector initial_points of a size specified by number_of_points. 
**/ 
vector< vector<double> > random_subsample( const vector< vector<double> >&  initial_points , size_t number_of_points )
{
	//copy vector of points
	vector< vector<double> > new_points( initial_points );
	
	//shuffle them randomy
	std::random_shuffle ( new_points.begin(), new_points.end() );
	
	//restrict the list to number_of_points positions. 
	new_points.resize( std::min( number_of_points , new_points.size() ) );
	
	return new_points;
}
