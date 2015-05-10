#ifndef CYLINDER_H
#define CYLINDER_H

#include <random>
#include <cmath>

#include "Particle.h"
#include "Settings.h"
#include "Macrobody.h"

class Cylinder: public Macrobody
{
public:
	//Ctor
	Cylinder(double h_in, double r_in, 
			 double x_in, double y_in, double z_in,
			 double absCS_in, double scatCS_in,
			 Settings& settings_in);

	bool isIn(Particle& particle);

	Macrobody::EventType Event(Particle& particle, 
			  				   std::default_random_engine& rng,
			  				   std::uniform_real_distribution<double>& prob);

private:
	////Private member variables////
	//Dimensions of cylinder
	double h;
	double r;

	//Position in space
	double x;
	double y;
	double z;

	//Material properties
	double absCS; //Abs step distance prob
	double scatCS; //Scatter Step distance prob

	//Settings
	Settings settings;
};

#endif