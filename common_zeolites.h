//Author: Pawel Dlotko
//Last modification: 13 Dec 2016
//Short descrption: A collection of functions that can be useful throughout the whole implementation.
//For internal use only!

#include <limits>


/**
 * This is a procedure to find min/max of a collection of points. 
**/ 

template< typename point_from_metric_space >
std::vector< std::pair< double,double > > find_ranges_of_point_cloud( const std::vector< point_from_metric_space >& points )
{	
	std::vector< std::pair< double,double > > result;
	if ( points.size() == 0 )return result;
	
	//in this case, the list points have at least one element. In particilar, we can chack what is the dimension of points.
	size_t dimension = points[0].size();
	
	result = std::vector< std::pair< double,double > >( dimension );
	for ( size_t i = 0 ; i != points[0].size() ; ++i )
	{
		result[i] = std::pair< double,double >( std::numeric_limits<double>::max() , -std::numeric_limits<double>::max() );
	}
	
	
	for ( size_t i = 0 ; i != points.size() ; ++i )
	{
		for ( size_t d = 0 ; d != dimension ; ++d )
		{
			if ( points[i][d] > result[d].second )
			{
				result[d] = std::pair<double,double>( result[d].first , points[i][d] );
			}
			if ( points[i][d] < result[d].first )
			{
				result[d] = std::pair<double,double>( points[i][d] , result[d].second );
			}
		}
	}
	
	return result;
}//find_ranges_of_point_cloud


void store_as_OFF_file( const std::vector< std::vector<double> >& points , const char* filename ) 
{
	std::ofstream out(filename);
	out << "OFF " << std::endl << points.size() << " 0 0" << std::endl;
	for ( size_t i = 0 ; i != points.size() ; ++i )
	{
		out << points[i][0] << " " << points[i][1] << " " << points[i][2] << std::endl;
	}
	out.close();
}


