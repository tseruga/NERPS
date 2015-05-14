#include "Material.h"

using namespace std;

Material::Material()
{}

Material::Material(string name_in, double density_in, 
				   double scatterProb_in, Settings* settings_in)
:name(name_in), density(density_in), 
	scatterProb(scatterProb_in), settings(settings_in){}

void Material::addIsotope(Isotope* isotope, int abundance)
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
	int totalAbundance = 0;
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
								   10e-22;

		double absProb = 1 - pow(2.71828182845904523536, 
								 -macroCrossSection*settings->step);
		//Absorption occurred
		if(roll < absProb)
			return Material::Absorb;

	}

	return Material::NoEvent;
}

void Material::printComposition()
{
	cout << "Material: " << name << endl;

	for (auto iso : composition)
	{
		cout << iso.isotope->getName() << endl;
		cout << "Abundance: " << iso.abundance << endl;
		cout << "Relative Abundance: " << iso.relativeAbundance << endl;
		cout << "Weight Fraction: " << iso.weightFraction << endl;
		cout << "Atomic Density: " << iso.atomicDensity << endl;
		cout << endl;
	}
}