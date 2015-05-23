#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <math.h>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <getopt.h>

#include "Settings.h"
#include "Particle.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Macrobody.h"
#include "Isotope.h"
#include "Material.h"

using namespace std;

void getOpt(int argc, char**argv, Settings& settings);

static inline void loadingBar(long long soFar, 
							  long long total,
							  bool timeMode);

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F func, Args&&... args)
    {
        auto start = std::chrono::system_clock::now();
        func(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT> 
                            (std::chrono::system_clock::now() - start);
        return duration.count();
    }
};

int main(int argc, char**argv)
{
	/////////////////Init Settings////////////////////
	Settings settings;
	getOpt(argc, argv, settings);
	
  	clock_t begin = clock();

  	long long particleCount = 0;

	//Initilization of output file
	ofstream out(settings.filename.c_str());
	ofstream runInfo("runInfo.txt");

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
	Isotope Al_27("Al_27", 27, "Al27-102.dat", 1.503);
	Isotope O_16("O_16", 16, "O16-10.dat", 4.232);
	Isotope Yb_168("Yb_168", 168, "Yb168-102.dat", 2.13);
	Isotope Yb_170("Yb_170", 170, "Yb170-102.dat", 5.8);
	Isotope Yb_171("Yb_171", 171, "Yb171-102.dat", 15.6);
	Isotope Yb_172("Yb_172", 172, "Yb172-102.dat", 11.2);
	Isotope Yb_173("Yb_173", 173, "Yb173-102.dat", 15);
	Isotope Yb_174("Yb_174", 174, "Yb174-102.dat", 46.8);
	Isotope Yb_176("Yb_176", 176, "Yb176-102.dat", 9.6);
	Isotope Vacuum("Vacuum", 0, "vacuum.dat", 0);

	cout << "Isotopes prepared.\n";
	cout << "Preparing materials...\n";

	////MATERIALS////
	Material Yb3O2("Ytterbium Oxide (Yb3O2)", 7.5, &settings);
	Yb3O2.addIsotope(&O_16, 2);
	Yb3O2.addIsotope(&Yb_168, 2.4);
	Yb3O2.addIsotope(&Yb_170, 0.0183);
	Yb3O2.addIsotope(&Yb_171, 0.0858);
	Yb3O2.addIsotope(&Yb_172, 0.1317);
	Yb3O2.addIsotope(&Yb_173, 0.0969);
	Yb3O2.addIsotope(&Yb_174, 0.1911);
	Yb3O2.addIsotope(&Yb_176, 0.0762);
	Yb3O2.prepare();

	Material alum("Aluminum", 2.702, &settings);
	alum.addIsotope(&Al_27, 1);
	alum.prepare();

	Material vacuum("Vacuum", 0, &settings);
	vacuum.addIsotope(&Vacuum, 1);
	vacuum.prepare();

	cout << "Materials prepared.\n";

	cout << "Placing macrobodies...\n";

	////MACROBODIES////
	vector<Macrobody*> macrobodies;

	//Aluminum LD Canister to hold pellets
	//Cylinder c1("Test Cylinder", 5, 10, 0, 0, 0, alum, 1, settings);
	Cone c1("Test Cone", 5, 5, 5, 0, 0, 0, 3, alum, 1, settings);
	macrobodies.push_back(&c1);

	cout << "Macrobodies placed.\n";
	cout << "Simulation is now running...\n";


	//The main loop that runs through all particle sims.
	for(auto i = 0; i < settings.numParticles; ++i)
	{
		if(settings.timeMode)
		{
			//Update time only ocassionally
			if(i%5000 == 0)
			{
				i=0;
				clock_t now = clock();
				double timeElapsed = double(now-begin)/CLOCKS_PER_SEC;
				if(timeElapsed/60. > settings.runningTime)
				{
					break;
				}
				//cout << (long long)((timeElapsed/60.)*10e3) << "/" << (long long)(settings.runningTime*10e3) << endl;
				loadingBar((int)((timeElapsed/60.)*10e3), (int)(settings.runningTime*10e3), settings.timeMode);
			}
		}
		else
		{
			//Print loading bar for the user
			loadingBar(i, settings.numParticles, settings.timeMode);
		}
		
		++particleCount;

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

	//Show the user a full loading bar
	loadingBar(99, 100, settings.timeMode);

	//Get time elapsed
	clock_t end = clock();
	double timeElapsed = double(end-begin)/CLOCKS_PER_SEC;

	cout << "\r" << flush;
	cout << "\n\nProcess was completed successfully\n";
	cout << "Absorption locations were written to file: " << settings.filename << "\n";
	cout << "Run info was written to file: runInfo.txt\n";

	runInfo << "Total time elapsed: " << timeElapsed/60. <<  " minutes" << endl;
	runInfo << "Particles ran: " << particleCount << endl;
	runInfo << "Lost to graveyard: " << graveyardCount << endl;
	runInfo << "Lost to absorption: " << absorptionCount << endl;
	runInfo << "Scatter events: " << scatterCount << endl;
	runInfo << "\n-------------MATERIALS-------------\n" << endl;
	runInfo << alum << Yb3O2;

	out.close();
	runInfo.close();
}

static inline void loadingBar(long long soFar, 
						 	  long long total,
						 	  bool timeMode)
{
	if(!timeMode)
	{
		//We're going to update on whole percentages
		if(soFar % (total/100 + 1) != 0) return;	
	}

	double ratio = soFar/(double)total;

	int c = ratio * 50 + 1;

	cout << setw(3) << (int)(ratio*100)+1 << "% [";
	for(int x = 0; x < c; x++) cout << "=";
	for(int x = c; x < 50; x++) cout << " ";
	cout << "]\r" << flush;

}

void getOpt(int argc, char**argv, Settings& settings)
{
	struct option longOpts[] = {
    { "time", required_argument, NULL, 't' },
    { "help", no_argument, NULL, 'h'},

    // We need this line so that getopt knows when to stop searching.
    // If we don't provide it, it will read beyond this array and crash
    // when we give it an unrecognized long option.
    { 0, 0, 0, 0 }
    };

    opterr = false;

    int opt = 0, index = 0;

    //Program defaults to particleCountMode
    settings.timeMode = false;

    while ((opt = getopt_long(argc, argv, "t:h", longOpts, &index)) != -1) {
        switch (opt) {
        case 't':
        	settings.timeMode = true;
        	settings.runningTime = atof(optarg);
            break;
        case 'h':
        	cout << "FLAGS:" << endl;
        	cout << "--time <arg> or -t <arg> : Runs the simulation for a set time given in minutes <arg>" << endl;
        	cout << "\t./sim -t 60 would run the program for one hour." << endl;
        	cout << endl;
        	cout << "Be sure to compile with \"make release\" to enable compiler optimization flags" << endl;
            exit(0);
            break;
        default:
            cout << "I didn't recognize one of your flags!\n";
            exit(1);
        }
    }

}