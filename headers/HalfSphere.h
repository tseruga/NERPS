#ifndef HALFSPHERE_H
#define HALFSPHERE_H

#include <random>
#include <cmath>

#include "Particle.h"
#include "Settings.h"
#include "Material.h"
#include "Macrobody.h"

class HalfSphere: public Macrobody
{
public:
	//Ctor
	HalfSphere(std::string name_in, 
			 double base_x_in, double base_y_in, double base_z_in,
			 double dir_x_in,  double dir_y_in,  double dir_z_in,
			 double r_in,
			 Material material_in,
			 int priority_in, 
			 Settings& settings_in);

	bool isIn(Particle& particle);

private:
	////Private member variables////
	//directional vector of half sphere
	std::vector<double> dir;

	//Position of base
	std::vector<double> base;

	//radius of half sphere
	double r;

	//Material properties
	double absCS; //Abs step distance prob
	double scatCS; //Scatter Step distance prob

	//Settings
	Settings settings;
};

#endif