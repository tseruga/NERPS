#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cmath>

//README
//These functions are for use with vector math in NeRPS
//The functions are:
//
// dot   -dotproduct
// mag   -magnitude
// dif   -subtraction
// add   -addition
// unit  -normalize vector
// cross -cross product

namespace vectorUtilities
{
	//returns dot product of vectors
    static double dot(std::vector<double>& a, std::vector<double>& b)
	{
		double dotProduct = 0;

		dotProduct = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];

		return dotProduct;
	}	

	//returns magnitude of vector
	static double mag(std::vector<double>& a)
	{
		double magnitude = 0;

		magnitude = a[0]*a[0]+a[1]*a[1]+a[2]*a[2];
	

		return sqrt( magnitude );
	}
	
	//subtract two vectors
	static std::vector<double> dif(std::vector<double>& a, std::vector<double>& b)
	{
		std::vector<double> c;

		c.push_back(a[0] - b[0]);
		c.push_back(a[1] - b[1]);
		c.push_back(a[2] - b[2]);

		return c;
	}

	//add two vectors
	static std::vector<double> add(std::vector<double>& a, std::vector<double>& b)
	{
		std::vector<double> c;

		c.push_back(a[0] + b[0]);
		c.push_back(a[1] + b[1]);
		c.push_back(a[2] + b[2]);

		return c;
	}

	//Normalizes a vector given to the function
	static std::vector<double> unit( std::vector<double>& a)
	{
		std::vector<double> c;
		double norm = vectorUtilities::mag(a);
		c.push_back(a[0]/norm);
		c.push_back(a[1]/norm);
		c.push_back(a[2]/norm);

		return c;
	}

	//multiplies a constant by a vector
	static std::vector<double> mult(double& k, std::vector<double>& a)
	{
		std::vector<double> c;
		c.push_back(a[0]*k);
		c.push_back(a[1]*k);
		c.push_back(a[2]*k);

		return c;		
	}

	//divides a constant by a vector
	static std::vector<double> div(double& k, std::vector<double>& a)
	{
		std::vector<double> c;
		c.push_back(a[0]/k);
		c.push_back(a[1]/k);
		c.push_back(a[2]/k);

		return c;		
	}

	//cross product of two vectors
	static std::vector<double> cross(std::vector<double>& a, std::vector<double>& b)
	{
		std::vector<double> c;

		c.push_back(a[1]*b[2] - a[2]*b[1]);
		c.push_back(a[2]*b[0] - a[0]*b[2]);
		c.push_back(a[0]*b[1] - a[1]*b[0]);

		return c;
	}

}




