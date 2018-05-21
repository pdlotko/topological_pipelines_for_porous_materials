//Author: Pawel Dlotko
//Last modification: 5 Oct 2016
//Short descrption: A function to find epsilon net for a given epsilon based on a distance matrix.
//For internal use only!

#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#include "common_zeolites.h"
#include "compute_distance_matrix_of_points_in_R3.h"

using namespace std;


/**
 * This is an algorithm to find epsilon net among given the lower triangular part of a distnace matrix and epsilon at the input.
 * Lower triangular part of a distance matrix consist of all the points in a distance matrix which are strictly below diagonal.
 * The algorithm proceed greedyly as follows: it pick the point representing the first row ofthe matrix and add it to the epsilon net.
 * Then it proceed to tne next points (rows) and add them, if they are at least epsilon away from the current epsilon net. 
 * If epsilon net do not exist, the algorithm will return the whole range between 0 and size of a matrix. 
**/ 
std::vector< size_t > find_epsilon_net( const std::vector< std::vector< double > >& lower_triangulad_distance_matrix , double epsilon )
{
	bool dbg = false;
	
	//vector of elemnetns that belong to the epsilon net	
	std::vector< size_t > result;
	result.push_back( 0 );
		
	for ( size_t i = 0 ; i != lower_triangulad_distance_matrix.size() ; ++i )
	{
		bool should_we_add_this_point_to_the_net = true;
		//check if column i is closer than epsilon to any element of the epsilon net or not.
		for ( size_t j = 0 ; j != result.size() ; ++j )
		{
			double local_distance = 0;
			if ( result[j] > i )
			{
				//local_distance = lower_triangulad_distance_matrix[i][ result[j] ];
				local_distance = lower_triangulad_distance_matrix[ result[j] ][ i ];
			}
			else
			{
				if ( result[j] != i )
				{
					//local_distance = lower_triangulad_distance_matrix[ result[j] ][i];
					local_distance = lower_triangulad_distance_matrix[ i ][ result[j] ];
				}
				else
				{
					local_distance = 0;
				}
			}
				
			if ( local_distance < epsilon )
			{
				if ( dbg )
				{
					std::cerr << "A distance between a point of a index : " << i << " and te point of an index : " << result[j] << " that belongs to the net is : " << local_distance << ". This point will not be added to the epsilon net" << std::endl;
				}
				should_we_add_this_point_to_the_net = false;
				break;
			}
		}
		if ( should_we_add_this_point_to_the_net )
		{
			result.push_back( i );
			if ( dbg )
			{
				std::cerr << "Adding point number : " << i << " to the epsilon net \n";
			}			
		}	
	}	
	return result;
}//find_epsilon_net


/**
 * This is a procedure that compute max-min subset of a given cardinality of a set. 
**/ 
std::vector< size_t > max_min_subset_certain_size( const std::vector< std::vector< double > >& lower_triangulad_distance_matrix , size_t size , bool deterministic = true )
{
	bool dbg = false;
	if ( dbg )std::cerr << "Entering max_min_subset_certain_size procedure \n";
	//vector of elemnetns that belong to the max-min set	
	
	std::vector< size_t > result;	
	std::vector< bool > belong_to_max_min_set( lower_triangulad_distance_matrix.size() , false );
	
	size_t first = 0;
	if ( !deterministic )
	{
		//in this case, the starting element is picked at random:
		srand( time(0) );
		first = (size_t)rand()%lower_triangulad_distance_matrix.size();		
	}
	result.push_back( first );
	belong_to_max_min_set[first] = true;
	if ( dbg )
	{
		std::cerr << "The first element of max-min set is : " << first << std::endl;
	}
	
	for ( size_t i = 1 ; i != size ; ++i )
	{
		//for every row of the distance matrix
		std::pair< size_t , double > pair_column_numer_and_distance_from_max_min_set = std::make_pair( 0 , -std::numeric_limits<double>::max() );
		for ( size_t j = 0 ; j != lower_triangulad_distance_matrix.size() ; ++j )
		{
			if ( dbg )
			{
				std::cerr << "Now working with a column : " << j << " of the distance matrix \n";
			}
			//check if it do not belong already to the max-min set:
			if ( belong_to_max_min_set[j] )continue;
			if ( dbg ){std::cerr << "This column do not belong to the constructed max-min set\n";}
							
			//if it do not belong, compute the distance to the max-min set:
			double minimal_distance = std::numeric_limits<double>::max();
			for ( size_t mm = 0 ; mm != result.size() ; ++mm )
			{				
				double local_distance = 0;
				if ( result[mm] > j )
				{
					//local_distance = lower_triangulad_distance_matrix[j][ result[mm] ];
					local_distance = lower_triangulad_distance_matrix[ result[mm] ][ j ];
				}
				else
				{
					if ( result[mm] != j )
					{
						//local_distance = lower_triangulad_distance_matrix[ result[mm] ][j];
						local_distance = lower_triangulad_distance_matrix[ j ][ result[mm] ];
					}
					else
					{
						local_distance = 0;
					}
				}				
				if ( minimal_distance > local_distance )minimal_distance = local_distance;
			}			
			if ( dbg )
			{
				std::cerr << "Distance of the point represneted by a column : " << j << " to the max-min set is : " << minimal_distance << std::endl;
				if (  minimal_distance>10000 )getchar();				
			}
			if ( pair_column_numer_and_distance_from_max_min_set.second < minimal_distance )
			{
				pair_column_numer_and_distance_from_max_min_set = std::make_pair( j , minimal_distance );
			}			
		}
		//now we should add pair_column_numer_and_distance_from_max_min_set.first to the max-min set:
		result.push_back( pair_column_numer_and_distance_from_max_min_set.first ); 
		belong_to_max_min_set[ pair_column_numer_and_distance_from_max_min_set.first ] = true;
		
		if ( dbg )
		{
			std::cerr << "The next element of max-min set is : " << pair_column_numer_and_distance_from_max_min_set.first << std::endl;
			std::cerr << "Its distance from the max-min set is : " << pair_column_numer_and_distance_from_max_min_set.second << std::endl;
			getchar();
		}
		
		if ( i == size-1 )
		{
			std::cerr << "The diameter of a net is : " << pair_column_numer_and_distance_from_max_min_set.second << std::endl;
		}
	}
	return result;
}

/**
 * Most of the procedures above operates on a distance matrices, and return indices of columns, not the points themselves. This is why, we need to convert indices of points to the points
 * for the further use in the program. The procedure below is designed to do that.
 * It has the following parameters:
 * const std::vector< std::vector<double> >& points -- initial points (used to create distance matrix).
 * const std::vector< size_t >& indices_of_points -- the indices of desired subset of the original points.
 * The procedure returns a subcolection of points enumerated by indices_of_points.
**/ 
std::vector< std::vector<double> > translate_incidences_of_points_to_points( const std::vector< std::vector<double> >& points , const std::vector< size_t >& indices_of_points )
{
	std::vector< std::vector<double> > result;
	for ( size_t i = 0 ; i != indices_of_points.size() ; ++i )
	{
		result.push_back( points[ indices_of_points[i] ] );
	}
	return result;
}//translate_incidences_of_points_to_points



/**
 * This is a procedure to find a subsample of a collection of points by using max_min procedure.
**/ 
std::vector< std::vector<double> > epsilon_net_based_subsample_procedure( const vector< vector<double> >& points , double diameter_of_epsilon_net )
{
	std::vector< std::vector< double > > distance = compute_distance_matrix_of_points_in_R3<euclidena_distance>( points );
	std::cout << "Now we will search for a min-max subset of a epsilon: " << diameter_of_epsilon_net << std::endl;	
	std::vector< size_t > indices_of_points = find_epsilon_net( distance , diameter_of_epsilon_net );
	std::cout << "Min-max subsample of a size : " << indices_of_points.size() << " have been found \n";
	std::vector< std::vector<double> > subsampled_points = translate_incidences_of_points_to_points( points , indices_of_points );
	return subsampled_points;
}


/**
 * This is a procedure to find a subsample of a collection of points by using max_min procedure that uses distance matrix computations. This is OK for not too large collections of points.
**/ 
std::vector< std::vector<double> > max_min_based_subsample_procedure( const vector< vector<double> >& points , size_t size_of_subsample )
{
	std::vector< std::vector< double > > distance = compute_distance_matrix_of_points_in_R3<euclidena_distance>( points );
	std::vector< size_t > subsampled_points_num = max_min_subset_certain_size( distance , size_of_subsample );
	std::vector< std::vector<double> > subsampled_points = translate_incidences_of_points_to_points(points ,  subsampled_points_num );	
	return subsampled_points;
}



/**
 * Auxiliary function to compute a distance from one point to all other points in the collection
**/ 
template< typename point_from_metric_space , typename distance_function >
std::vector< double > compute_distance_from_point_to_any_other_point( const std::vector< point_from_metric_space >& points , point_from_metric_space point )
{
	std::vector< double > result( point.size() , 0 );
	distance_function f;
	
	for ( size_t i = 0 ; i != points.size() ; ++i )
	{
		result.push_back( f( point, points[i] ) );
	}
	
	return result;
}//compute_distance_from_point_to_any_other_point



/**
 * This is a procedure that compute max-min subset of a given cardinality of a set. Unlike in the previous procedure, where the input is a distance matrix, here we accept 
 * points of any metric space, and a function to compute distance between those points. This procedure should be used instead of the one that accept in the input the distance
 * matrix, if this distance matrix is too large to compute. 
**/ 
template< typename point_from_metric_space , typename distance_function >
std::vector< point_from_metric_space > max_min_subset_certain_size( const std::vector< point_from_metric_space >& points , size_t size , bool deterministic = true )
{
	bool dbg = false;
	if ( dbg ){std::cerr << "Entering max_min_subset_certain_size procedure (the one that accept the points from metric space).\n";getchar();}
	//vector of elemnetns that belong to the max-min set			
	
	std::vector< point_from_metric_space > result;	
	std::vector< bool > belong_to_max_min_set( points.size() , false );
	
	size_t first = 0;
	if ( !deterministic )
	{
		//in this case, the starting element is picked at random:
		srand( time(0) );
		first = (size_t)rand()%points.size();		
	}
	result.push_back( points[first] );
	belong_to_max_min_set[first] = true;
	
	//this is a structure to keep the distances from the set of landmarks to all other points:
	std::vector< std::vector< double > > distances;
	
	//now we compute a distance from the first point to any other point in the collection:
	point_from_metric_space pt = points[ first ];
	std::vector< double > dist_ = compute_distance_from_point_to_any_other_point<point_from_metric_space , distance_function>( points , pt );
	distances.push_back( dist_ );
	
	size--;
	while ( size )
	{
		size_t farthest_poin = 0;
		double distance_to_the_farthest_point = -std::numeric_limits<double>::max();
		
		//find a point which is the farthest one from any point in result vector:
		for ( size_t i = 0 ; i != points.size() ; ++i )
		{
			if ( belong_to_max_min_set[i] )continue;
			double minimal_distance_to_max_min_set = std::numeric_limits<double>::max();
			//here we have to find a minimal distance of this point from the points already in the max-min set:
			for ( size_t point_in_max_min = 0 ; point_in_max_min != distances.size() ; ++point_in_max_min )
			{
				if ( minimal_distance_to_max_min_set > distances[point_in_max_min][i] )minimal_distance_to_max_min_set = distances[point_in_max_min][i];
			}
			if ( distance_to_the_farthest_point < minimal_distance_to_max_min_set ) 
			{
				distance_to_the_farthest_point = minimal_distance_to_max_min_set;
				farthest_poin = i;
			}
		}
		
		if ( dbg )
		{
			std::cerr << "farthest_poin : " << farthest_poin << std::endl;
			std::cerr << "Distance to the farthest_poin : " << distance_to_the_farthest_point << std::endl;
			getchar();
		}
		
		//compute distance from the farthest_poin to any other point:
		pt = points[ farthest_poin ];
		belong_to_max_min_set[ farthest_poin ] = true;
		std::vector< double > dist_2 = compute_distance_from_point_to_any_other_point<point_from_metric_space , distance_function>( points , pt );
		distances.push_back( dist_2 );
		result.push_back( points[ farthest_poin ] );				
		size--;
	}
	
	if ( dbg )
	{
		std::cerr << "Found a set of landmark points of a size : " << result.size() << std::endl;
	}
	
	return result;
}


/**
 * This procedure takes as an input: min and max of a grid, and number of points in all direction. It computers the number of cell as if it was in a bitmap cubical complex
**/
template< typename point_from_metric_space > 
size_t compute_cell_number( const point_from_metric_space& point , const std::vector< std::pair< double,double > > min_max , size_t number_of_cubes_direction_x , size_t number_of_cubes_direction_y , size_t number_of_cubes_direction_z  )
{
	bool dbg = false;
	size_t x_coord = ( ( point[0] - min_max[0].first )/( min_max[0].second - min_max[0].first ) ) * number_of_cubes_direction_x;
	size_t y_coord = ( ( point[1] - min_max[1].first )/( min_max[1].second - min_max[1].first ) ) * number_of_cubes_direction_y;
	size_t z_coord = ( ( point[2] - min_max[2].first )/( min_max[2].second - min_max[2].first ) ) * number_of_cubes_direction_z;
	
	if ( dbg )
	{
		std::cerr << "( ( point[0] - min_max[0].first )/( min_max[0].second - min_max[0].first ) ) : " << ( ( point[0] - min_max[0].first )/( min_max[0].second - min_max[0].first ) ) << endl;
		std::cerr << "( ( point[1] - min_max[1].first )/( min_max[1].second - min_max[1].first ) ) : " << ( ( point[1] - min_max[1].first )/( min_max[1].second - min_max[1].first ) ) << endl;
		std::cerr << "( ( point[1] - min_max[1].first )/( min_max[2].second - min_max[2].first ) ) : " << ( ( point[2] - min_max[2].first )/( min_max[2].second - min_max[2].first ) ) << endl;
		std::cerr << "Considering a point : " << point[0] << " " << point[1] << " " << point[2] << std::endl;
		std::cerr << "Its coordinates are : " << x_coord << " " << y_coord << " " << z_coord << std::endl;
	}
	
	//and now we should compute the position of this point:
	size_t result = number_of_cubes_direction_x*number_of_cubes_direction_y * z_coord +
	                number_of_cubes_direction_x * y_coord + x_coord;
	                
	if ( dbg )
	{
		std::cerr << "result : " << result << std::endl;
	}
	
	return result;
}//compute_cell_number

template< typename point_from_metric_space >
bool sort_according_second_coord( const std::pair<point_from_metric_space,size_t>& first , const std::pair<point_from_metric_space,size_t>& second )
{
	return first.second < second.second;
}//srt_according_second_coord


/**
 * This procedure finds a subsample of a given point cloud by puting a grid on the initial point cloud. In this case, the grid have a fixed number of cubes. From each cube, if there are points from point cloud in it
 * a fixed nuber of poitns in selected. Note that this procedure do not guarantee the number of points in the subsample.
**/ 
template< typename point_from_metric_space >
std::vector< point_from_metric_space > subsample_by_using_grid( const std::vector< point_from_metric_space >& points , size_t number_of_cubes_direction_x , size_t number_of_cubes_direction_y , size_t number_of_cubes_direction_z , size_t how_many_points_to_sample_from_cube  )
{
	bool dbg = false;
	std::vector< std::pair<point_from_metric_space,size_t> > points_to_bins_vector;
	points_to_bins_vector.reserve( points.size() );
	
	std::vector< std::pair< double,double > > min_max = find_ranges_of_point_cloud<point_from_metric_space>( points );
	if ( dbg )
	{
		std::cerr << "Here are mix-maxes :\n";
		for ( size_t i = 0 ; i != min_max.size() ; ++i )
		{
			std::cout << min_max[i].first << " " << min_max[i].second << std::endl;
		}
		std::cerr << "End \n";
		getchar();
	}
	
	//fill in the points_to_bins_vector vector. 
	for ( size_t i = 0 ; i != points.size() ; ++i )
	{		
		if (dbg)cerr << points[i][0] << " " << points[i][1] << " " << points[i][2] << endl;
		size_t cell_no = compute_cell_number( points[i] , min_max , number_of_cubes_direction_x , number_of_cubes_direction_y , number_of_cubes_direction_z );
		points_to_bins_vector.push_back( std::pair< point_from_metric_space , size_t >(points[i] , cell_no ) );
		if ( dbg )
		{		
			cerr << "cell_no : " << cell_no << std::endl;	
			getchar();		
		}
	}	
	
	//sort the points_to_bins_vector accodring to the number of cell the belong to
	std::sort( points_to_bins_vector.begin() , points_to_bins_vector.end() , sort_according_second_coord<point_from_metric_space> );
	
	if ( dbg )
	{
		for ( size_t i = 0 ; i != points_to_bins_vector.size() ; ++i )
		{
			std::cout << points_to_bins_vector[i].first[0] << " "<< points_to_bins_vector[i].first[1] << " "<< points_to_bins_vector[i].first[2] << " " << points_to_bins_vector[i].second << std::endl;
			if ( i%10 == 0 )getchar();
		}
	}
	
	//for every cell, check how many points we have, and sample how_many_points_to_sample_from_cube from it. 
	std::vector< point_from_metric_space > result;
	size_t position = 0;
	while ( position < points_to_bins_vector.size() )
	{
		//find the next position:
		size_t next_position = position+1;
		while ( ( next_position < points_to_bins_vector.size() ) && ( points_to_bins_vector[position].second == points_to_bins_vector[next_position].second ) )
		{
			++next_position;
		}
		
		//check how many elements are in this bin:
		size_t how_many_elements_in_this_bin = next_position - position;
		
		if ( dbg )
		{
			std::cout << "position : " << position << ", next_position : " << next_position << std::endl;
			std::cout << "how_many_elements_in_this_bin : " << how_many_elements_in_this_bin << std::endl;
		}
		
		
		//now, compare this number with how_many_points_to_sample_from_cube
		if ( how_many_elements_in_this_bin > how_many_points_to_sample_from_cube )
		{
			//in this case we sample from the range. We should do it by doing some random_shuffle.
			std::vector< size_t > to_sample;
			to_sample.reserve( next_position - position );
			for ( size_t i = position ; i != next_position ; ++i )to_sample.push_back( i );
			std::random_shuffle( to_sample.begin() , to_sample.begin() + how_many_points_to_sample_from_cube );			
			for ( size_t i = 0 ; i != how_many_points_to_sample_from_cube ; ++i )result.push_back( points_to_bins_vector[ to_sample[i] ].first );
		}
		else
		{
			//in this case, we use all of them. 
			for ( size_t i = position ; i != next_position ; ++i )result.push_back( points_to_bins_vector[i].first );
		}
		
		if ( dbg )
		{
			std::cout << "result.size() : " << result.size() << std::endl;
			getchar();
		}
		
		position = next_position;
	}
	
	
	
	return result;
}//subsample_by_using_grid

/** 
* This procedure do a subsampling taking into account parameters of the unit cell. Its parameters are: vector of points, maximal guaranteed Hausdorff distance of the subsampe from the initial poit cloud
* and the sizes of the unit cell in directions x, y and z. 
**/
template< typename point_from_metric_space >
std::vector< point_from_metric_space > subsample_by_using_grid_Hausorff_distance_guarantee( const std::vector< point_from_metric_space >& points , double maximal_hausdorff_distance_of_subsample_from_sample , 
																							double size_of_cell_x_direction , double size_of_cell_y_direction , double size_of_cell_z_direction )
{
	bool dbg = true;
	size_t number_of_cubes_in_x_direction = 2*(size_t)(size_of_cell_x_direction/maximal_hausdorff_distance_of_subsample_from_sample);
	size_t number_of_cubes_in_y_direction = 2*(size_t)(size_of_cell_y_direction/maximal_hausdorff_distance_of_subsample_from_sample);
	size_t number_of_cubes_in_z_direction = 2*(size_t)(size_of_cell_z_direction/maximal_hausdorff_distance_of_subsample_from_sample);
	
	if ( dbg )
	{
	
		std::cerr << "subsample_by_using_grid_Hausorff_distance_guarantee procedure, " << endl;
		std::cerr << number_of_cubes_in_x_direction << " " << number_of_cubes_in_y_direction << " " << number_of_cubes_in_z_direction << std::endl;
		std::cerr << "size_of_cell_x_direction : " << size_of_cell_x_direction << std::endl;
		std::cerr << "size_of_cell_y_direction : " << size_of_cell_y_direction << std::endl;
		std::cerr << "size_of_cell_z_direction : " << size_of_cell_z_direction << std::endl; 
		getchar();
	}
	
	return subsample_by_using_grid( points , number_of_cubes_in_x_direction , number_of_cubes_in_y_direction , number_of_cubes_in_z_direction , 1  );
}//subsample_by_using_grid_Hausorff_distance_guarantee
