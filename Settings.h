#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

class Settings
{
public:
	Settings();

	//Delta T for particle updating
	double step;

	//Size of the spawning sphere
	double sphereR;

	//Number of particles
	long long numParticles;

	//File name of output
	std::string filename;

private:
	void read();
};

#endif