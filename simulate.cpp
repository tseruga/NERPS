#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <math.h>
#include <iomanip>

#include "Settings.h"
#include "Particle.h"
#include "Cylinder.h"
#include "Macrobody.h"
#include "Isotope.h"
#include "Material.h"

using namespace std;

static inline void loadingBar(long long soFar, 
							  long long total);

int main()
{
	/////////////////Init Settings////////////////////
	Settings settings;

	//Initilization of output file
	ofstream out(settings.filename.c_str());

	//Initialization of rng
	default_random_engine rng(time(NULL));
	uniform_real_distribution<double> rngTheta(0, 2*M_PI);
	uniform_real_distribution<double> rngPhi(0, M_PI);
	uniform_real_distribution<double> rngVel(-1.,1.);
	//[TODO] Change this to the proper distribution for energy of particles
	uniform_real_distribution<double> rngEn(1, 2e13);

	//Initialization of macrobodies
	vector<Macrobody*> macrobodies;

	//Single cylinder centered at the origin
	Cylinder c1(1.75, 0.6, 0, 0, 0, 9.4953, 0.027628, settings);
	macrobodies.push_back(&c1);

	long long graveyardCount = 0;
	long long scatterCount = 0;
	long long absorptionCount = 0;

	////////TESTING//////////
	Isotope O_16("O_16", 16, "O_16.dat");
	Isotope H_1("H_1", 1, "H_1.dat");
	Material water("water", 1);
	water.addIsotope(&O_16, 1);
	water.addIsotope(&H_1, 2);
	water.prepare();
	water.printComposition();

	cout << "Simulation is now running...\n";

	//The main loop that runs through all particle sims.
	for(auto i = 0; i < settings.numParticles; ++i)
	{
		//Print loading bar for the user
		loadingBar(i, settings.numParticles);

		//Spawn a particle
		Particle particle(rng, rngTheta, rngPhi, rngVel, rngEn, &settings);

		while(particle.isAlive())
		{
			Macrobody::EventType lastEvent = Macrobody::NoEvent;

			//Check if particle is in contact with something
			for(auto body : macrobodies)
			{
				//If the particle is in contact
				if(body->isIn(particle))
				{
					//Then calculate what happens
					lastEvent = body->Event(particle, rng, rngVel); 
					//As of now, a particle can only be in one body
					//[TODO] Add priorities to macrobodies
					break; 
				}
			}

			//Add to counters for certain events.
			//Update particle on scatter or no event.
			switch (lastEvent)
			{
				case Macrobody::Absorb:
					++absorptionCount;
					//Print to file here
					out << particle << "\n";
					break;
				case Macrobody::Scatter:
					++scatterCount;
					//No break, as we want scatters to update too
				default:
					particle.Update();
			}

			//Particle entered the graveyard
			if(lastEvent != Macrobody::Absorb && !particle.isAlive())
			{
				++graveyardCount;
			}
		}
	}

	cout << "\r" << flush;
	cout << "\n\nProcess was completed successfully\n";
	cout << "Results were written to file: " << settings.filename << "\n\n";
	cout << "----------Statistics----------\n";
	cout << "Particles ran: " << settings.numParticles << endl;
	cout << "Lost to graveyard: " << graveyardCount << endl;
	cout << "Lost to absorption: " << absorptionCount << endl;
	cout << "Scatter events: " << scatterCount << endl;

	out.close();
}

static inline void loadingBar(long long soFar, 
						 	  long long total)
{
	//We're going to update on whole percentages
	if(soFar % (total/100 + 1) != 0) return;

	double ratio = soFar/(double)total;

	int c = ratio * 50 + 1;

	cout << setw(3) << (int)(ratio*100)+1 << "% [";
	for(int x = 0; x < c; x++) cout << "=";
	for(int x = c; x < 50; x++) cout << " ";
	cout << "]\r" << flush;

}