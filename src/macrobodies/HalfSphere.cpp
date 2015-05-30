#include "HalfSphere.h"
#include "VectorUtilities.h"

using namespace std;
using namespace vectorUtilities;

HalfSphere::HalfSphere(std::string name_in, 
			 double base_x_in, double base_y_in, double base_z_in,
			 double dir_x_in,  double dir_y_in,  double dir_z_in,
			 double r_in,
			 Material material_in,
			 int priority_in, 
			 Settings& settings_in)
:r(r_in), settings(settings_in)
{
	name = name_in;
	material = material_in;
	priority = priority_in;

	//Creating vectors
	base.push_back(base_x_in);
	base.push_back(base_y_in);
	base.push_back(base_z_in);

	dir.push_back(dir_x_in);
	dir.push_back(dir_y_in);
	dir.push_back(dir_z_in);

	dir = unit(dir);

}

bool HalfSphere::isIn(Particle& particle)
{
	//first thing is to make particle's location a vector
	vector<double> partLoc;
	partLoc.push_back(particle.getX());
	partLoc.push_back(particle.getY());
	partLoc.push_back(particle.getZ());


	//Vector from base to particle to be tested
	vector<double> baseToPart = dif(partLoc,base);

	//first check projection along dir

	double h = dot(baseToPart, dir);

	//if dot product is negative is not in sphere

	if(h<0)
	{
		return false;
	}

	//if dot product is greater than radius than is not in sphere

	if(h>r)
	{
		return false;
	}

	//test radius of point now

	if( mag(baseToPart) < r )
	{
		return true;
	}

	return false;

}