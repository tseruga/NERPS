#include "Cylinder.h"

using namespace std;

Cylinder::Cylinder(double h_in, double r_in, 
			 	   double x_in, double y_in, double z_in,
			 	   Material material_in,
			 	   Settings& settings_in)
:h(h_in), r(r_in), 
x(x_in), y(y_in), z(z_in),
settings(settings_in)
{
	material = material_in;
}

bool Cylinder::isIn(Particle& particle)
{
	//Check the x/y plane first
	double square_dist = (particle.getX()-x) * (particle.getX()-x) +
						 (particle.getY()-y) * (particle.getY()-y);

	//If particle is within cylinder on x/y plane
	if(square_dist < r * r)
	{
		//If particle is within the z plane of the cylinder
		if((particle.getZ()-z) < h/2 && (particle.getZ()-z) > -h/2)
			return true;
	}

	return false;
}