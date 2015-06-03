#ifndef MACROBODY_H
#define MACROBODY_H

#include <random>
#include <math.h>

#include "Particle.h"
#include "Settings.h"
#include "Material.h"

// Abstract base class used to define all types of Macrobodies
class Macrobody
{
public:
	//Returns true if the particle is contained within the 
	//macrobody's geometry. False otherwise.
	virtual bool isIn(Particle& particle) = 0;

	//Calcuates the event that occurs when the particle is in contact
	//with the macrobody. Can either scatter, absorb, or die without absorption.
	//Returns the EventType that occurred
	virtual Material::EventType event(Particle& particle, 
		  			   		std::default_random_engine& rng,
		  			   		std::uniform_real_distribution<double>& prob)
	{
		Material::EventType val = material.event(particle, rng, prob);

		switch(val)
		{
			case Material::Absorb:
			{
				particle.kill();
				break;
			}
			case Material::Scatter:
			{

				std::uniform_real_distribution<double> rngVel(-1.,1.);
				std::vector<double> newVel(3);

				newVel[0] = rngVel(rng);
				newVel[1] = rngVel(rng);
				newVel[2] = rngVel(rng);

				double normNew = sqrt( (newVel[0]*newVel[0]) + 
								       (newVel[1]*newVel[1]) + 
								       (newVel[2]*newVel[2]) );

				newVel[0] = newVel[0]/normNew;
				newVel[1] = newVel[1]/normNew;
				newVel[2] = newVel[2]/normNew;

				particle.setVelocity(newVel);
				break;
			}
			default:
			{
				break;
			}
		}

		return val;
	}

	virtual int getPriority(){return priority;}

	virtual std::string getName(){return name;}

protected:
	std::string name;
	int priority;
	Material material;

};

#endif