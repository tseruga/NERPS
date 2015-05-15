#ifndef BOX_H
#define BOX_H

#include <random>
#include <cmath>

#include "Particle.h"
#include "Settings.h"
#include "Macrobody.h"

class Box: public Macrobody
{
public:
	//Ctor
	Box(std::string name_in, 
		double x_length_in, 
		double y_length_in, double z_length_in,
		double x_in, double y_in, double z_in,
		Material material_in,
		int priority_in, 
		Settings& settings_in);

	bool isIn(Particle& particle);

private:
	////Private member variables////
	//Dimensions of side length of box
	double x_l;
	double y_l;
	double z_l;

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