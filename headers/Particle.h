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
	Particle(double x_in, double y_in, double z_in, std::vector<double> velocity_in, double energy_in,
			 Settings* settings_in);

	////Public member functions////
	
	//Steps the particle forward in time
	//Also kills the particle if it goes out of sphere
	void update();

	//Kills this particle
	void kill(){alive = false;}

	bool isAlive(){return alive;}

	long long getScatterCount(){return scatterCount;}
	long long gettickCount(){return tickCount;}

	double getEnergy(){return energy;}

	//Getters
	double getX() {return x;}
	double getY() {return y;}
	double getZ() {return z;}

	//Setters
	void setVelocity( std::vector<double> a ); //also adds one to scatter count
	void setEnergy(double a) {energy = a;}

	friend std::ostream& operator<<(std::ostream& os, const Particle& particle);

private:
	////Private member variables////
	Settings* settings;
	double x;
	double y;
	double z;
	std::vector<double> velocity;
	double energy;
	bool alive;
	bool absorbed;
	long long scatterCount;
	long long tickCount;
};

#endif