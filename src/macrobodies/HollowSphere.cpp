#include "HollowSphere.h"

using namespace std;

HollowSphere::HollowSphere(string name_in,
			   double r_outer_in, double r_inner_in, 
			   double x_in, double y_in, double z_in,
			   Material material_in,
			   int priority_in, 
			   Settings& settings_in)
:rOuter(r_outer_in), rInner(r_inner_in),
x(x_in), y(y_in), z(z_in),
settings(settings_in)
{
	name = name_in;
	material = material_in;
	priority = priority_in;
}

bool HollowSphere::isIn(Particle& particle)
{
	//Gets the square of the paritcles distance from the sphere
	double square_dist = (particle.getX()-x) * (particle.getX()-x) +
						 (particle.getY()-y) * (particle.getY()-y) +
						 (particle.getZ()-z) * (particle.getZ()-z);

	//If the particle is within the sphere
	if(square_dist > rOuter * rOuter)
	{
		return false;
	}

	if(square_dist < rInner*rInner)
	{
		return false;
	}

	return true;
}