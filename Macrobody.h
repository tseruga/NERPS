#ifndef MACROBODY_H
#define MACROBODY_H

#include <random>

#include "Particle.h"
#include "Settings.h"

// Abstract base class used to define all types of Macrobodies
class Macrobody
{
public:

	enum EventType {NoEvent, Scatter, Absorb, Graveyard};

	//Returns true if the particle is contained within the 
	//macrobody's geometry. False otherwise.
	virtual bool isIn(Particle& particle) = 0;

	//Calcuates the event that occurs when the particle is in contact
	//with the macrobody. Can either scatter, absorb, or die without absorption.
	//Returns the EventType that occurred
	virtual EventType Event(Particle& particle, 
		  			   		std::default_random_engine& rng,
		  			   		std::uniform_real_distribution<double>& prob) = 0;

};

#endif