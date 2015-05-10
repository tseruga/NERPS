#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
	//Delta T for particle updating
	double step;

	//Size of the spawning sphere
	double sphereR;

	//Number of particles
	long long numParticles;
};

#endif