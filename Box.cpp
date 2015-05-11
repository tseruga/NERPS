#include "Box.h"

using namespace std;

Box::Box( double x_length_in, 
			 double y_length_in, double z_length_in,
			 double x_in, double y_in, double z_in,
			 double absCS_in, double scatCS_in,
			 Settings& settings_in)
:x_l(x_length_in), y_l(y_length_in), z_l(z_length_in), x(x_in), y(y_in), z(z_in),
settings(settings_in)
{
	absCS = 1 - pow(2.71828182845904523536, -absCS_in*settings.step);
	scatCS = 1 - pow(2.71828182845904523536, -scatCS_in*settings.step);
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


Macrobody::EventType Box::Event(Particle& particle, 
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
