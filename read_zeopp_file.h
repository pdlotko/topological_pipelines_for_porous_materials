//Author: Pawel Dlotko
//Last modification: 5 Oct 2016
//Short descrption: A function to read points from a zeo++ file to a vector< vector<double> > and to write a vector of points in R^3 into a file.
//For internal use only!

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <cstdlib>
#include <cstring>

using namespace std;


/**
 * This program read a collection of points from a zeo++ file. The path to a file is given in the input parameter filename. 
**/ 
vector< vector<double> > read_points_from_zeopp_file( const char* filename , bool exclude_inaccessible_pores = true )
{
	bool dbg = false;
	//first check if the file exist:
	std::ifstream in;
    in.open( filename );
	if ( !( access( filename, F_OK ) != -1 ) )
	{
		cerr << "The file : " << filename << " do not exist. The program will now terminate \n";
		throw "The file from which you are trying to read the persistence landscape do not exist. The program will now terminate \n";
	}
	std::string line;
	double x, y, z;
	double number;
	char character;
	
	vector< vector<double> > points;
    
    while ( in.good() )
    {
		getline(in,line);
		std::stringstream lineSS;
        lineSS << line;
		lineSS >> x >> y >> z >> number >> character;
		if ( exclude_inaccessible_pores == true )  		
		{
			if ( character != 'a' )continue;//in this case, the pore is not accessible
		}
		
		
		if ( !in.good() )break;
		std::vector< double > v(3);
		v[0] = x;
		v[1] = y;
		v[2] = z;
		
		if ( dbg )
		{
			std::cerr << "Reading point : " << x << " " << y << " " << z << std::endl;
			//getchar();
		}
		
		points.push_back( v );	
	}
	
	return points;
}




/**
 * This program read a collection of points in R^3 from a a file. Each point is a line
**/ 
vector< vector<double> > read_points_from_file( const char* filename )
{
	bool dbg = false;
	//first check if the file exist:
	std::ifstream in;
    in.open( filename );
	if ( !( access( filename, F_OK ) != -1 ) )
	{
		cerr << "The file : " << filename << " do not exist. The program will now terminate \n";
		throw "The file from which you are trying to read the persistence landscape do not exist. The program will now terminate \n";
	}
	std::string line;
	double x, y, z;
	double number;
	char character;
	
	vector< vector<double> > points;
    
    while ( in.good() )
    {
		getline(in,line);
		std::stringstream lineSS;
        lineSS << line;
		lineSS >> x >> y >> z;						
		if ( !in.good() )break;
		std::vector< double > v(3);
		v[0] = x;
		v[1] = y;
		v[2] = z;			
		points.push_back( v );	
	}
	
	return points;
}




/**
 * This procedure assume that const vector< vector<double> >& points is a vector of triples representing points in R^3. It create a file named filename and write the points to this file in a format readable 
 * by vesta.
**/ 
void write_points_to_file_in_vesta_format( const vector< vector<double> >& points , const char* filename )
{
	ofstream out;
	
	stringstream ss;
	ss << filename << ".xyz";
	out.open( ss.str().c_str() );
	
	out << points.size() << std::endl;
	
	out << endl; //empty line
	
	//std::cerr << "Writing : " << points.size() << " points to a file : " << filename <<std::endl;getchar();
	
	for ( size_t i = 0 ; i != points.size() ; ++i )
	{
		out << "1 " << points[i][0] << " " << points[i][1] << " " << points[i][2] << std::endl;
	}
	
	out.close();
}//write_points_to_file

/**
 * This procedure assume that const vector< vector<double> >& points is a vector of triples representing points in R^3. It create a file named filename and write the points to this file.
**/ 
void write_points_to_file( const vector< vector<double> >& points , const char* filename )
{
	ofstream out;
	out.open( filename );
	
	for ( size_t i = 0 ; i != points.size() ; ++i )
	{
		out << points[i][0] << " " << points[i][1] << " " << points[i][2] << std::endl;
	}
	
	out.close();
}//write_points_to_file


/**
 * This procedure reads the information about unit cell from zeopp file. The information are returned as a vector of doubles in the following order:
 * length of the first vector, length of the second vector, length of the third vector, angle between first and second vector, angle between first and third vector, 
 * angle between second and third vector.
**/
std::vector< double > read_information_about_unit_cell_from_zeopp_file( const char* filename )
{
	bool dbg = false;
	
	double length_first, length_second, length_third;
	double angle_1, angle_2, angle_3;
	
	//check if file exist
	std::ifstream in;
    in.open( filename );
	if ( !( access( filename, F_OK ) != -1 ) )
	{
		cerr << "The file : " << filename << " do not exist. The program will now terminate \n";
		throw "The file from which you are trying to read the persistence landscape do not exist. The program will now terminate \n";
	}
	
	in >> length_first >> length_second >> length_third >> angle_1 >> angle_2 >> angle_3;
	
	if ( dbg )
	{
		std::cerr << "length_first : " << length_first << std::endl;
		std::cerr << "length_second : " << length_second << std::endl;
		std::cerr << "length_third : " << length_third << std::endl;
		std::cerr << "angle_1 : " << angle_1 << std::endl;
		std::cerr << "angle_2 : " << angle_2 << std::endl;
		std::cerr << "angle_3 : " << angle_3 << std::endl;
	}
	
	in.close();
	
	std::vector< double > result;
	result.push_back(length_first);
	result.push_back(length_second);
	result.push_back(length_third);
	result.push_back(angle_1);
	result.push_back(angle_2);
	result.push_back(angle_3);
	
	return result;
}//read_information_about_unit_cell_from_zeopp_file
 
/**
 * This procedure read an accessible surface area from .sa file. This file need to be created before using this procedure by running a command:
 * "./network -ha -sa 1.3 1.3 2000 " << argv[1] << ".sa " << argv[1] << ".cssr";	
**/
double get_accessible_surface_area_from_sa_file( const char* filename )
{		
	//opening file
	std::ifstream t;
	t.open( filename );	
	//reading it to a string:
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());	
	unsigned first = str.find("ASA_A^2:");
	unsigned last = str.find("ASA_m^2/cm^3:");
	first += 9;// we shift it by the length of a string ASA_A^2:		
	string strNew = str.substr (first,last-first); 		
	stringstream ss;
	ss << strNew;
	double result;
	ss >> result;	
	std::cerr << "The accessible surface area is : " << result << std::endl;
	return result;
}


/**
 * This procedure store mega cell vector in a file
**/ 
void write_mega_cell_vector_to_a_file( const std::vector< std::vector< double > >& mega_cell_vector , const char* filename )
{
	ofstream out(filename);
	for ( size_t i = 0 ; i != mega_cell_vector.size() ; ++i )
	{
		for ( size_t j = 0 ; j != mega_cell_vector[i].size() ; ++j )
		{
			out << mega_cell_vector[i][j] << " ";
		}
		out << std::endl;
	}
	out.close();
}//write_mega_cell_vector_to_a_file

/**
 * This procedure read mega cell vector from a file
**/
std::vector< std::vector< double > > read_mega_cell_vector_from_a_file( const char* filename )
{
	ifstream in(filename);
	if ( !in.good() )
	{
		std::cerr << "The file " << filename << " do not exist, the program will now terminate. \n";
		throw "The file do not exist, the program will now terminate. \n";
	}
	std::vector< std::vector< double > > result;
	//those are 3 numbers in 3 lines
	double l1x, l1y, l1z;
	double l2x, l2y, l2z;
	double l3x, l3y, l3z;
	
	in >> l1x >> l1y >> l1z;
	in >> l2x >> l2y >> l2z;
	in >> l3x >> l3y >> l3z;
	
	std::vector< double > v1(3);
	v1[0] = l1x;
	v1[1] = l1y;
	v1[2] = l1z;
	
	std::vector< double > v2(3);
	v2[0] = l2x;
	v2[1] = l2y;
	v2[2] = l2z;	
	
	std::vector< double > v3(3);
	v3[0] = l3x;
	v3[1] = l3y;
	v3[2] = l3z;
	
	result.push_back(v1);
	result.push_back(v2);
	result.push_back(v3);
		
	return result;
}//read_mega_cell_vector_from_a_file	
