#ifndef HOLLOWSPHERE_H
#define HOLLOWSPHERE_H

#include <random>
#include <cmath>

#include "Particle.h"
#include "Settings.h"
#include "Material.h"
#include "Macrobody.h"

class HollowSphere: public Macrobody
{
public:
	//Ctor
	HollowSphere(std::string name_in,
		   double r_outer_in, double r_inner_in, 
		   double x_in, double y_in, double z_in,
		   Material material_in,
		   int priority_in, 
		   Settings& settings_in);

	bool isIn(Particle& particle);

private:
	////Private member variables////
	//Dimensions of Sphere
	double rOuter;
	double rInner;

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