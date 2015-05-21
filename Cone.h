#ifndef CONE_H
#define CONE_H

#include <random>
#include <cmath>

#include "Particle.h"
#include "Settings.h"
#include "Material.h"
#include "Macrobody.h"

class Cone: public Macrobody
{
public:
	//Ctor
	Cone(std::string name_in,
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

	//radius of base
	double r;

	//Cone angle/apperture 
	double halfAlpha;

	//Components of the vector from the apex to the base
	std::vector<double> apexToBase;

	//Material properties
	double absCS; //Abs step distance prob
	double scatCS; //Scatter Step distance prob

	//Settings
	Settings settings;

	//helper functions

	//strait forward dot product of a and b
	double dot(std::vector<double> a, std::vector<double> b);

	//magnitude of a
	double mag(std::vector<double> a);

	//differance between a and b
	std::vector<double> dif(std::vector<double> a, std::vector<double> b);

};

#endif