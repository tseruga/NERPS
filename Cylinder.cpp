#include "Cylinder.h"

using namespace std;

Cylinder::Cylinder(double h_in, double r_in, 
			 	   double x_in, double y_in, double z_in,
			 	   double absCS_in, double scatCS_in,
			 	   Settings& settings_in)
:h(h_in), r(r_in), 
x(x_in), y(y_in), z(z_in),
settings(settings_in)
{
	absCS = 1 - pow(2.71828182845904523536, -absCS_in*settings.step);
	scatCS = 1 - pow(2.71828182845904523536, -scatCS_in*settings.step);
}

bool Cylinder::isIn(Particle& particle)
{
	//Check the x/y plane first
	double square_dist = (x-particle.getX()) * (x-particle.getX()) +
						 (y-particle.getY()) * (y-particle.getY());

	//If particle is within cylinder on x/y plane
	if(square_dist < r * r)
	{
		//If particle is within the z plane of the cylinder
		if(particle.getZ() < h/2 && particle.getZ() > -h/2)
			return true;
	}

	return false;
}


void Cylinder::Event(Particle& particle, 
		  			default_random_engine& rng,
		  			uniform_real_distribution<double>& prob)
{
	//Check for absorption
	double roll = abs(prob(rng)); //Prob here is actually the velocity rng
						   		  //This prevents some issues with creating another rng
						   		  //So just use the abs() since rngVel [-1,1]

	//If it was absorbed
	if(roll < absCS)
	{
		particle.Kill();
		return;
	}

	//Check for scatter
	roll = abs(prob(rng)); //Prob here is actually the velocity rng
						   //This prevents some issues with creating another rng
						   //So just use the abs() since rngVel [-1,1]
	//If it was scattered
	if(roll < scatCS)
	{
		particle.Scatter(rng, prob);
		return;
	}

	return;
}
