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
			 double apex_x_in, double apex_y_in, double apex_z_in, 
			 double base_x_in, double base_y_in, double base_z_in,
			 double r_in,
			 Material material_in,
			 int priority_in, 
			 Settings& settings_in);

	bool isIn(Particle& particle);

private:
	////Private member variables////
	//location of the apex
	std::vector<double> apex;

	//Position of base
	std::vector<double> base;

	//radius of cylinder
	double r;

	//height of cylinder 
	double height;

	//Components of the vector from the apex to the base
	std::vector<double> baseToApex;

	//Material properties
	double absCS; //Abs step distance prob
	double scatCS; //Scatter Step distance prob

	//Settings
	Settings settings;
};

#endif