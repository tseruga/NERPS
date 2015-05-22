#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cmath>

namespace vectorUtilities
{
    static double dot(std::vector<double> a, std::vector<double> b)
	{
		double dotProduct = 0;

		dotProduct = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];

		return dotProduct;
	}	

	static double mag(std::vector<double> a)
	{
		double magnitude = 0;

		magnitude = a[0]*a[0]+a[1]*a[1]+a[2]*a[2];
	

		return sqrt( magnitude );
	}
	
	static std::vector<double> dif(std::vector<double> a, std::vector<double> b)
	{
		std::vector<double> c;

		c.push_back(a[0] - b[0]);
		c.push_back(a[1] - b[1]);
		c.push_back(a[2] - b[2]);

		return c;
	}
}
