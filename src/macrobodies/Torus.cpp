#include "Torus.h"
#include "VectorUtilities.h"

using namespace std;
using namespace vectorUtilities;

Torus::Torus(std::string name_in,
			 double center_x_in, double center_y_in, double center_z_in, 
			 double dir_x_in, double dir_y_in, double dir_z_in, //Directional vector of Torus !CAN NOT BE ZERO VECTOR!
			 double r1_in, //inner radius
			 double r2_in, //outer radius !CAN NOT BE 0!
			 Material material_in,
			 int priority_in, 
			 Settings& settings_in)
:r1(r1_in), r2(r2_in), settings(settings_in)
{
	name = name_in;
	material = material_in;
	priority = priority_in;

	//Creating vectors
	center.push_back(center_x_in);
	center.push_back(center_y_in);
	center.push_back(center_z_in);

	dir.push_back(dir_x_in);
	dir.push_back(dir_y_in);
	dir.push_back(dir_z_in);

	dir = unit(dir);

	r3 = (r2 - r1)/2;
}

bool Torus::isIn(Particle& particle)
{
	//first thing is to make particle's location a vector
	vector<double> partLoc;
	partLoc.push_back(particle.getX());
	partLoc.push_back(particle.getY());
	partLoc.push_back(particle.getZ());

	//From center to particle
	vector<double> centerToPart = dif(partLoc, center);

	//First thing to to test distance along dir
	double h1 = dot(centerToPart, dir);

	if( h1 > r3 or -1*h1 < -1*r3 )
	{
		return false;
	}

	//then we test its radius

	double r4 = sqrt( pow(mag(centerToPart), 2) - h1*h1 );
	double rp = r4 - (r1+r3);
	if( rp*rp + h1*h1 < r3*r3)
	{
		return true;
	}

	return false;

}