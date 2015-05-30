#include "Sphere.h"

using namespace std;

Sphere::Sphere(string name_in,
			   double r_in, 
			   double x_in, double y_in, double z_in,
			   Material material_in,
			   int priority_in, 
			   Settings& settings_in)
:r(r_in), 
x(x_in), y(y_in), z(z_in),
settings(settings_in)
{
	name = name_in;
	material = material_in;
	priority = priority_in;
}

bool Sphere::isIn(Particle& particle)
{
	//Gets the square of the paritcles distance from the sphere
	double square_dist = (particle.getX()-x) * (particle.getX()-x) +
						 (particle.getY()-y) * (particle.getY()-y) +
						 (particle.getZ()-z) * (particle.getZ()-z);

	//If the particle is within the sphere
	if(square_dist < r * r)
	{
			return true;
	}

	return false;
}