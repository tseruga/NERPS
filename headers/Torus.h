#ifndef TORUS_H
#define TORUS_H

#include <random>
#include <cmath>

#include "Particle.h"
#include "Settings.h"
#include "Material.h"
#include "Macrobody.h"

class Torus: public Macrobody
{
public:
	//Ctor
	Torus(std::string name_in,
			 double center_x_in, double center_y_in, double center_z_in, 
			 double dir_x_in, double dir_y_in, double dir_z_in, //Directional vector of Torus !CAN NOT BE ZERO VECTOR!
			 double r1_in, //inner radius
			 double r2_in, //outer radius !CAN NOT BE 0!
			 Material material_in,
			 int priority_in, 
			 Settings& settings_in);

	bool isIn(Particle& particle);

private:
	////Private member variables////
	//location of the center
	std::vector<double> center;

	//directional vector
	std::vector<double> dir;

	//radi
	double r1;
	double r2;

	//Radius of inside of the torus 
	double r3;

	//Material properties
	double absCS; //Abs step distance prob
	double scatCS; //Scatter Step distance prob

	//Settings
	Settings settings;
};

#endif