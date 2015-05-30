#include "Cylinder.h"
#include "VectorUtilities.h"

using namespace std;
using namespace vectorUtilities;

Cylinder::Cylinder(std::string name_in,
			 double apex_x_in, double apex_y_in, double apex_z_in, 
			 double base_x_in, double base_y_in, double base_z_in,
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
	apex.push_back(apex_x_in);
	apex.push_back(apex_y_in);
	apex.push_back(apex_z_in);

	base.push_back(base_x_in);
	base.push_back(base_y_in);
	base.push_back(base_z_in);

	baseToApex = dif(apex,base);



	height = mag(baseToApex);
}

bool Cylinder::isIn(Particle& particle)
{
	//first thing is to make particle's location a vector
	vector<double> partLoc;
	partLoc.push_back(particle.getX());
	partLoc.push_back(particle.getY());
	partLoc.push_back(particle.getZ());


	//Vector from base to particle to be tested
	vector<double> baseToPart = dif(partLoc,base);

	//projection of baseToPart on baseToApex

	double h = dot(baseToPart, baseToApex) / mag(baseToApex);

	//if dot product is negative is not in cylinder

	if(h<0)
	{
		return false;
	}

	//if dot product is greater than height than is not in cylinder

	if(h>height)
	{
		return false;
	}

	//if radius from center is greater than of cylinder, return false

	double particleDist = mag(baseToPart); //Distance of particle from base center

	if((r*r) > (particleDist*particleDist) - (h*h) )
	{
		return true;
	}

	return false;

}