#include "Material.h"

using namespace std;

Material::Material(string name_in, double density_in)
:name(name_in), density(density_in){}

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
	return NoEvent;
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