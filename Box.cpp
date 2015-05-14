#include "Box.h"

using namespace std;

Box::Box( double x_length_in, 
			 double y_length_in, double z_length_in,
			 double x_in, double y_in, double z_in,
			 Material material_in,
			 Settings& settings_in)
:x_l(x_length_in), y_l(y_length_in), z_l(z_length_in), x(x_in), y(y_in), z(z_in),
settings(settings_in)
{
	material = material_in;
}

bool Box::isIn(Particle& particle)
{
	//Gets the x distance from box center
	double x_dist = (particle.getX() - x);

	//If the particle is within two x planes
	if(x_dist * x_dist > ((x_l*x_l)/4))
	{
			return false;
	}

	//Gets the y distance from box
	double y_dist = (particle.getY() - y);

	//If the particle is within two x planes
	if(y_dist * y_dist > ((y_l*y_l)/4))
	{
			return false;
	}

	//Gets the y distance from box
	double z_dist = (particle.getZ() - z);

	//If the particle is within two x planes
	if(z_dist * z_dist > ((z_l*z_l)/4))
	{
			return false;
	}

	return true;
}