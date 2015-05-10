#include "Particle.h"

using namespace std;

Particle::Particle(vector<double>& xPos,
		 		   vector<double>& yPos,
				   vector<double>& zPos,
		 		   vector<vector<double>>& vels,
		 		   Settings& settings_in)
{
	//Set the initial position and velocity
	x = xPos.back();
	xPos.pop_back();

	y = yPos.back();
	yPos.pop_back();

	z = zPos.back();
	zPos.pop_back();

	velocity = vels.back();
	vels.pop_back();

	//Bring it to life
	alive = true;
	absorbed = false;
	scatterCount = 0;

	//Save settings
	settings = settings_in;
}

void Particle::Update()
{
	//Move the particle relative to its velocity and step
	x = x + velocity[0]*settings.step;
	y = y + velocity[1]*settings.step;
	z = z + velocity[2]*settings.step;

	//Check if we're out of bounds
	double rho = sqrt(x*x + y*y + z*z);
	if(rho > settings.sphereR)
	{
		alive = false;
		return;
	}

}


void Particle::Scatter(default_random_engine& rng,
			 		   uniform_real_distribution<double>& rngVel)
{
	//Calculate a new velocity unit vector
	vector<double> newVel(3);

	newVel[0] = rngVel(rng);
	newVel[1] = rngVel(rng);
	newVel[2] = rngVel(rng);

	double norm = sqrt( (newVel[0]*newVel[0]) + (newVel[1]*newVel[1]) + (newVel[2]*newVel[2]) );

	newVel[0] = newVel[0]/norm;
	newVel[1] = newVel[1]/norm;
	newVel[2] = newVel[2]/norm;

	velocity = newVel;

	scatterCount++;
}

ostream& operator<<(ostream& os, const Particle& particle)
{
	os << particle.x << "," << particle.y << "," << particle.z;
	return os;
}