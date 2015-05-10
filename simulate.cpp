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

using namespace std;

static inline void loadingBar(long long ran, 
							 long long totalToRun);

int main()
{
	//[TODO] Remove this
	cout << "Input name of file to write to: ";
	string filename;
	cin >> filename;

	/////////////////Init Settings////////////////////
	//[TODO] Get settings from text file
	Settings settings;
	settings.numParticles = 4000000;
	settings.sphereR = 5;
	settings.step = 0.05;

	//Initilization of output file
	ofstream out(filename.c_str());

	//Initialization of rng
	default_random_engine rng(time(NULL));

	//For efficiency, calculate all base rng ahead of time
	uniform_real_distribution<double> rngTheta(0, 2*M_PI);
	uniform_real_distribution<double> rngPhi(0, M_PI);
	uniform_real_distribution<double> rngVel(-1.,1.);

	//Initialization of macrobodies
	vector<Macrobody*> macrobodies;

	//Single cylinder centered at the origin
	Cylinder c1(1.75, 0.6, 0, 0, 0, 9.4953, 0.027628, settings);
	macrobodies.push_back(&c1);

	long long graveyardCount = 0;
	long long scatterCount = 0;
	long long absorptionCount = 0;

	cout << "Simulation is now running...\n";

	//The main loop that runs through all particle sims.
	for(auto i = 0; i < settings.numParticles; ++i)
	{
		//Print loading bar for the user
		loadingBar(i, settings.numParticles);

		//Spawn a particle
		Particle particle(rng, rngTheta, rngPhi, rngVel, settings);

		while(particle.isAlive())
		{
			//Check if particle is in contact with something
			for(auto body : macrobodies)
			{
				//If the particle is in contact
				if(body->isIn(particle))
				{
					//Then calculate what happens
					body->Event(particle, rng, rngVel); 
					//As of now, a particle can only be in one body
					//[TODO] Add priorities to macrobodies
					break; 
				}
			}

			//If particle has been absorbed
			if(!particle.isAlive())
			{
				++absorptionCount;
				scatterCount += particle.getScatterCount();

				//Print to file here
				out << particle << "\n";
				break;
			}
			else
			{
				//Step the particle forward in time
				particle.Update();
			}

			//Particle entered the graveyard
			if(!particle.isAlive())
			{
				scatterCount += particle.getScatterCount();
				++graveyardCount;
			}
		}
	}

	cout << "\r" << flush;
	cout << "\n\nProcess was completed successfully\n";
	cout << "Results were written to file: " << filename << "\n\n";
	cout << "----------Statistics----------\n";
	cout << "Particles ran: " << settings.numParticles << endl;
	cout << "Lost to graveyard: " << graveyardCount << endl;
	cout << "Lost to absorption: " << absorptionCount << endl;
	cout << "Scatter events: " << scatterCount << endl;

	out.close();
}

static inline void loadingBar(long long ran, 
						 	 long long totalToRun)
{
	//We're going to update on whole percentages
	if(ran % (totalToRun/100 + 1) != 0) return;

	double ratio = ran/(double)totalToRun;

	int c = ratio * 50 + 1;

	cout << setw(3) << (int)(ratio*100)+1 << "% [";
	for(int x = 0; x < c; x++) cout << "=";
	for(int x = c; x < 50; x++) cout << " ";
	cout << "]\r" << flush;

}