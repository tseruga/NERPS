#include "Sphere.h"

using namespace std;

Sphere::Sphere( double r_in, 
			 	   double x_in, double y_in, double z_in,
			 	   double absCS_in, double scatCS_in,
			 	   Settings& settings_in)
:r(r_in), 
x(x_in), y(y_in), z(z_in),
settings(settings_in)
{
	absCS = 1 - pow(2.71828182845904523536, -absCS_in*settings.step);
	scatCS = 1 - pow(2.71828182845904523536, -scatCS_in*settings.step);
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


Macrobody::EventType Sphere::Event(Particle& particle, 
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
		return Macrobody::Absorb;
	}

	//Check for scatter
	roll = abs(prob(rng)); //Prob here is actually the velocity rng
						   //This prevents some issues with creating another rng
						   //So just use the abs() since rngVel [-1,1]
	//If it was scattered
	if(roll < scatCS)
	{
		particle.Scatter(rng, prob);
		return Macrobody::Scatter;
	}

	return Macrobody::NoEvent;
}
