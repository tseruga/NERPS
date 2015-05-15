#include "Material.h"

using namespace std;

Material::Material()
{}

Material::Material(string name_in, double density_in, 
				   double crossSection, Settings* settings_in)
:name(name_in), density(density_in), settings(settings_in)
{
	scatterProb = 1 - pow(2.71828182845904523536, 
								 -crossSection*settings->step);
}

void Material::addIsotope(Isotope* isotope, double abundance)
{
	//Start the isotope info for the isotope
	IsotopeInfo iso;
	iso.isotope = isotope;
	iso.abundance = abundance;

	//Add the isotope to the composition of the material
	composition.push_back(iso);
}

void Material::prepare()
{
	double totalAbundance = 0;
	for(auto iso : composition)
	{
		totalAbundance += iso.abundance;
	}

	for (unsigned i = 0; i < composition.size(); ++i)
	{
		composition[i].relativeAbundance = (double)composition[i].abundance /
										   (double)totalAbundance;
	}

	double totalWeight = 0;
	for (unsigned i = 0; i < composition.size(); ++i)
	{
		totalWeight += composition[i].relativeAbundance * 
					   composition[i].isotope->getAtomicWeight();
	}

	for (unsigned i = 0; i < composition.size(); ++i)
	{
		composition[i].weightFraction = ( composition[i].relativeAbundance * 
										  composition[i].isotope->getAtomicWeight() ) / 
										  totalWeight;

		composition[i].avoAmu = 6.02e23 / 
								(double)composition[i].isotope->getAtomicWeight();
		
		composition[i].atomicDensity = density * 
									   composition[i].weightFraction * 
									   composition[i].avoAmu;
	}
}

Material::EventType Material::event(Particle& particle, 
		  				  			default_random_engine& rng,
		  				  			uniform_real_distribution<double>& prob)
{
	double roll;
	//Go through each isotope in the composition
	//If an event occurs, return that event type
	for(auto iso : composition)
	{
		//Check scatter first
		roll = abs(prob(rng));
		//Scatter occurred
		if(roll < scatterProb)
			return Material::Scatter;

		//Check absorption
		roll = abs(prob(rng));
		double macroCrossSection = iso.atomicDensity * 
								   iso.isotope->getCrossSection(particle.getEnergy()) *
								   10e-25;

		double absProb = 1 - pow(2.71828182845904523536, 
								 -macroCrossSection*settings->step);
		//Absorption occurred
		if(roll < absProb)
			return Material::Absorb;

	}

	return Material::NoEvent;
}

ostream& operator<<(ostream& os, const Material& material)
{
	os << "------ Material: " << material.name << " ------" << endl;

	for (auto iso : material.composition)
	{
		os << iso.isotope->getName() << endl;
		os << "Abundance: " << iso.abundance << endl;
		os << "Relative Abundance: " << iso.relativeAbundance << endl;
		os << "Weight Fraction: " << iso.weightFraction << endl;
		os << "Atomic Density: " << iso.atomicDensity << " atoms/cm^3" << endl;
		os << endl;
	}

	return os;
}