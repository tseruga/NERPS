#include "Cone.h"

using namespace std;

Cone::Cone(std::string name_in,
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

	apexToBase = dif(base, apex);

	//Defining angles 
	halfAlpha = tan(r/(mag(dif(base,apex))));
}

bool Cone::isIn(Particle& particle)
{
	//first thing is to make particle's location a vector
	vector<double> partLoc;
	partLoc.push_back(particle.getX());
	partLoc.push_back(particle.getY());
	partLoc.push_back(particle.getZ());


	//The anlge the particle makes with the axes vector shoul be less than halfAlpha

	//Vector from apex to particle to be tested
	vector<double> apexToPart = dif(partLoc,apex);

	//angle of particle is given by dot product.
	//dot prouct of vector from apex to particle and from apex to base
	
	//dotParticle divided by the magnitude of apexToBase and apexToParticle
	bool isInInfiniteCone = dot(apexToPart, apexToBase)/(mag(apexToPart)*mag(apexToBase)) > cos(halfAlpha);

	if(!isInInfiniteCone)
	{
		return false;
	}

	bool isUnderBase = dot(apexToPart, apexToBase)/(mag(apexToBase)) < mag(apexToBase);

	return isUnderBase;

}

double Cone::dot(vector<double> a, vector<double> b)
{
	double dotProduct = 0;

	dotProduct = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];

	return dotProduct;
}

double Cone::mag(vector<double> a)
{
	double magnitude = 0;

	magnitude = a[0]*a[0]+a[1]*a[1]+a[2]*a[2];
	

	return sqrt( magnitude );
}

vector<double> Cone::dif(vector<double> a, vector<double> b)
{
	vector<double> c;

	c.push_back(a[0] - b[0]);
	c.push_back(a[1] - b[1]);
	c.push_back(a[2] - b[2]);

	return c;
}