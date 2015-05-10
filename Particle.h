#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <random>

#include "Settings.h"

class Particle
{
public:
	//Ctor
	Particle(std::default_random_engine& rng,
			 std::uniform_real_distribution<double>& rngTheta,
			 std::uniform_real_distribution<double>& rngPhi,
			 std::uniform_real_distribution<double>& rngVel,
			 Settings settings_in);

	////Public member functions////
	
	//Steps the particle forward in time
	//Also kills the particle if it goes out of sphere
	void Update();

	//Calculates a new random velocity vector for the particle
	void Scatter(std::default_random_engine& rng,
			     std::uniform_real_distribution<double>& rngVel);

	//Kills this particle
	void Kill(){alive = false;}

	bool isAlive(){return alive;}

	long long getScatterCount(){return scatterCount;}

	//Getters
	double getX() {return x;}
	double getY() {return y;}
	double getZ() {return z;}

	friend std::ostream& operator<<(std::ostream& os, const Particle& particle);

private:
	////Private member variables////
	Settings settings;
	double x;
	double y;
	double z;
	std::vector<double> velocity;
	bool alive;
	bool absorbed;
	long long scatterCount;
};

#endif