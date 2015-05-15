#ifndef CYLINDER_H
#define CYLINDER_H

#include <random>
#include <cmath>

#include "Particle.h"
#include "Settings.h"
#include "Material.h"
#include "Macrobody.h"

class Cylinder: public Macrobody
{
public:
	//Ctor
	Cylinder(std::string name_in,
			 double h_in, double r_in, 
			 double x_in, double y_in, double z_in,
			 Material material_in,
			 int priority_in, 
			 Settings& settings_in);

	bool isIn(Particle& particle);

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