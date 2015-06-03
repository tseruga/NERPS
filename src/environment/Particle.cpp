#include "Particle.h"

using namespace std;

Particle::Particle(double x_in, double y_in, double z_in, std::vector<double> velocity_in, double energy_in,
			 Settings* settings_in)
:x(x_in), y(y_in), z(z_in), velocity(velocity_in), energy(energy_in)
{
	//Save settings
	settings = settings_in;

	//Bring it to life
	alive = true;
	absorbed = false;
	scatterCount = 0;
	tickCount = 0;

}

void Particle::update()
{
	//Move the particle relative to its velocity and step
	x = x + velocity[0]*settings->step;
	y = y + velocity[1]*settings->step;
	z = z + velocity[2]*settings->step;

	//tick
	tickCount ++;

	//Check if we're out of bounds
	double rho = sqrt(x*x + y*y + z*z);
	if(rho > settings->sphereR)
	{
		alive = false;
		return;
	}

}

void Particle::setVelocity(std::vector<double> a)
{
	velocity = a;
	scatterCount ++;
}

ostream& operator<<(ostream& os, const Particle& particle)
{
	os << particle.x << "," << particle.y << "," << particle.z;
	return os;
}