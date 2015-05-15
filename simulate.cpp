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

	//Initialization of energy rng (a bit more involved)
	ifstream weightFile("Weights/weights10k");
	vector<double> weights;
	double weight;
	weightFile >> weight;
	while(!weightFile.fail())
	{
		weights.push_back(weight);
		weightFile >> weight;
	}

	vector<double> intervals;
	for(double i = 0; i <= 10; i = i + 0.001)
	{
		intervals.push_back(i);
	}

	piecewise_constant_distribution<double> rngEn(intervals.begin(), intervals.end(), weights.begin());

	////////SETTING UP THE SIMULATION UNIVERSE////////

	long long graveyardCount = 0;
	long long scatterCount = 0;
	long long absorptionCount = 0;

	cout << "Preparing isotopes...\n";

	////ISOTOPES////
	Isotope Al_27("Al_27", 27, "Al27-102.dat");
	Isotope O_16("O_16", 16, "O16-10.dat");
	Isotope Yb_168("Yb_168", 168, "Yb168-102.dat");
	Isotope Yb_170("Yb_170", 170, "Yb170-102.dat");
	Isotope Yb_171("Yb_171", 171, "Yb171-102.dat");
	Isotope Yb_172("Yb_172", 172, "Yb172-102.dat");
	Isotope Yb_173("Yb_173", 173, "Yb173-102.dat");
	Isotope Yb_174("Yb_174", 174, "Yb174-102.dat");
	Isotope Yb_176("Yb_176", 176, "Yb176-102.dat");
	Isotope Vacuum("Vacuum", 0, "vacuum.dat");

	cout << "Isotopes prepared.\n";
	cout << "Preparing materials...\n";

	////MATERIALS////
	Material Yb3O2("Ytterbium Oxide (Yb3O2)", 7.5, 0.027628, &settings);
	Yb3O2.addIsotope(&O_16, 2);
	Yb3O2.addIsotope(&Yb_168, 2.4);
	Yb3O2.addIsotope(&Yb_170, 0.0183);
	Yb3O2.addIsotope(&Yb_171, 0.0858);
	Yb3O2.addIsotope(&Yb_172, 0.1317);
	Yb3O2.addIsotope(&Yb_173, 0.0969);
	Yb3O2.addIsotope(&Yb_174, 0.1911);
	Yb3O2.addIsotope(&Yb_176, 0.0762);
	Yb3O2.prepare();

	Material alum("Aluminum", 2.702, 9.06e-3, &settings);
	alum.addIsotope(&Al_27, 1);
	alum.prepare();

	Material vacuum("Vacuum", 0, 0, &settings);
	vacuum.addIsotope(&Vacuum, 1);
	vacuum.prepare();

	cout << "Materials prepared.\n";
	cout << "Placing macrobodies...\n";

	////MACROBODIES////
	vector<Macrobody*> macrobodies;

	//Aluminum LD Canister to hold pellets
	Cylinder ld("LD Canister", 30, 4.75, 0, 0, 0, alum, 2, settings);
	macrobodies.push_back(&ld);

	//Vacuum channels to hold pellets
	Cylinder channel1("Vacuum Channel #1", 22.5, 0.405, 0, 2.31, 0, vacuum, 3, settings);
	macrobodies.push_back(&channel1);

	Cylinder channel2("Vacuum Channel #2", 22.5, 0.405, 1.63342, 1.63342, 0, vacuum, 3, settings);
	macrobodies.push_back(&channel2);

	Cylinder channel3("Vacuum Channel #3", 22.5, 0.405, 2.31, 0, 0, vacuum, 3, settings);
	macrobodies.push_back(&channel3);

	Cylinder channel4("Vacuum Channel #4", 22.5, 0.405, 1.63342, -1.63342, 0, vacuum, 3, settings);
	macrobodies.push_back(&channel4);

	Cylinder channel5("Vacuum Channel #5", 22.5, 0.405, 0, -2.31, 0, vacuum, 3, settings);
	macrobodies.push_back(&channel5);

	Cylinder channel6("Vacuum Channel #6", 22.5, 0.405, -1.63342, -1.63342, 0, vacuum, 3, settings);
	macrobodies.push_back(&channel6);

	Cylinder channel7("Vacuum Channel #7", 22.5, 0.405, -2.31, 0, 0, vacuum, 3, settings);
	macrobodies.push_back(&channel7);

	Cylinder channel8("Vacuum Channel #8", 22.5, 0.405, -1.63342, 1.63342, 0, vacuum, 3, settings);
	macrobodies.push_back(&channel8);

	//Yb3O2 Pellets placed inside of vacuum channels
	Cylinder pellet1("Yb3O2 Pellet #1", 21, 0.3, 0, 2.31, -0.875, Yb3O2, 4, settings);
	macrobodies.push_back(&pellet1);

	Cylinder pellet2("Yb3O2 Pellet #2", 21, 0.3, 1.63342, 1.63342, -0.875, Yb3O2, 4, settings);
	macrobodies.push_back(&pellet2);

	Cylinder pellet3("Yb3O2 Pellet #3", 21, 0.3, 2.31, 0, -0.875, Yb3O2, 4, settings);
	macrobodies.push_back(&pellet3);

	Cylinder pellet4("Yb3O2 Pellet #4", 21, 0.3, 1.63342, -1.63342, -0.875, Yb3O2, 4, settings);
	macrobodies.push_back(&pellet4);

	Cylinder pellet5("Yb3O2 Pellet #5", 21, 0.3, 0, -2.31, -0.875, Yb3O2, 4, settings);
	macrobodies.push_back(&pellet5);

	Cylinder pellet6("Yb3O2 Pellet #6", 21, 0.3, -1.63342, -1.63342, -0.875, Yb3O2, 4, settings);
	macrobodies.push_back(&pellet6);

	Cylinder pellet7("Yb3O2 Pellet #7", 21, 0.3, -2.31, 0, -0.875, Yb3O2, 4, settings);
	macrobodies.push_back(&pellet7);

	Cylinder pellet8("Yb3O2 Pellet #8", 21, 0.3, -1.63342, 1.63342, -0.875, Yb3O2, 4, settings);
	macrobodies.push_back(&pellet8);

	cout << "Macrobodies placed.\n";
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
			Material::EventType lastEvent = Material::NoEvent;
			Macrobody* highestPriorityBody = NULL;
			int highestPriorityNum = -1;

			//Check if particle is in contact with something
			for(auto body : macrobodies)
			{
				//If the particle is in contact
				if(body->isIn(particle))
				{
					if(body->getPriority() > highestPriorityNum)
					{
						highestPriorityBody = body;
						highestPriorityNum = body->getPriority();
					}
					break; 
				}
			}

			//Calculate event on highest priority body in current contact
			if(highestPriorityNum != -1)
				lastEvent = highestPriorityBody->event(particle, rng, rngVel); 

			//Add to counters for certain events.
			//Update particle on scatter or no event.
			switch (lastEvent)
			{
				case Material::Absorb:
					++absorptionCount;
					//Print to file here
					out << highestPriorityBody->getName() << "," << 
						particle << "\n";
					break;
				case Material::Scatter:
					++scatterCount;
					//No break, as we want scatters to update too
				default:
					particle.update();
			}

			//Particle entered the graveyard
			if(lastEvent != Material::Absorb && !particle.isAlive())
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