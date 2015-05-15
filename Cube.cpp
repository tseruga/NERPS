#include "Cube.h"

using namespace std;

Cube::Cube(string name_in,
		   double a_in, 
		   double x_in, double y_in, double z_in,
		   Material material_in,
		   int priority_in, 
		   Settings& settings_in)
:a(a_in), 
x(x_in), y(y_in), z(z_in),
settings(settings_in)
{
	name = name_in;
	material = material_in;
	priority = priority_in;
}

bool Cube::isIn(Particle& particle)
{
	//Gets the x distance from cube center
	double x_dist = (particle.getX() - x);

	//If the particle is within two x planes
	if(x_dist * x_dist > ((a*a)/4) )
	{
			return false;
	}

	//Gets the y distance from cube
	double y_dist = (particle.getY() - y);

	//If the particle is within two x planes
	if(y_dist * y_dist > ((a*a)/4) )
	{
			return false;
	}

	//Gets the y distance from cube
	double z_dist = (particle.getZ() - z);

	//If the particle is within two x planes
	if(z_dist * z_dist > ((a*a)/4) )
	{
			return false;
	}

	return true;
}
