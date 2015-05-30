#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <cmath>

#include "Particle.h"
#include "Isotope.h"
#include "Settings.h"

class Material
{
	public:
		enum EventType {NoEvent, Scatter, Absorb, Graveyard};

		Material();

		Material(std::string name_in, double density_in, 
				 Settings* settings_in);

		//Adds isotope to the composition of the material
		void addIsotope(Isotope* isotope, double abundance);

		//To be called after all isotopes have been added to the material
		//Calculates needed information about the material
		void prepare();

		//Determines whether an event occurred within a material
		EventType event(Particle& particle, 
		  				std::default_random_engine& rng,
		  				std::uniform_real_distribution<double>& prob);

		friend std::ostream& operator<<(std::ostream& os, const Material& material);

	private:

		std::string name;
		double density;

		//Holds all the information about a specific isotope
		//relative to the rest of the isotopes in a material.
		//
		//This is done here because an isotope may be used across
		//several materials.
		struct IsotopeInfo
		{
			Isotope* isotope;
			double abundance;
			double relativeAbundance;
			double weightFraction;
			double avoAmu;
			double atomicDensity;
		};

		std::vector<IsotopeInfo> composition;

		double scatterProb;

		Settings* settings;
};

#endif