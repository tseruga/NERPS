#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <math.h>
#include <iomanip>

#include "Settings.h"
#include "Particle.h"
#include "Cylinder.h"

using namespace std;

static inline void printInfo(long long ran, 
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

	//Vector to hold all the calculated x values
	vector<double> xPos;

	//Vector to hold all the calculated y values
	vector<double> yPos;

	//Vector to hold all the calcuated z values
	vector<double> zPos;

	//Vector to hold all the calculated velocity vectors
	vector<vector<double>> vels;

	cout << "Initializing particles... \n(this may take a while)\n";

	//Fill the above vectors
	for(auto i = 0; i < settings.numParticles; ++i)
	{
		//Get spherical coords
		double theta = rngTheta(rng);
		double phi = rngPhi(rng);
		double rho = settings.sphereR;

		//Convert to cartersian coords
		xPos.push_back(rho*sin(theta)*cos(phi));
		yPos.push_back(rho*sin(theta)*sin(phi));
		zPos.push_back(rho*cos(theta));

		//Velocity
		double xV = rngVel(rng);
		double yV = rngVel(rng);
		double zV = rngVel(rng);

		//Normalize to unit vector
		double norm = sqrt(xV*xV + yV*yV + zV*zV);
		xV = xV / norm;
		yV = yV / norm;
		zV = zV / norm;

		vector<double> tempVel(3);
		tempVel[0] = xV;
		tempVel[1] = yV;
		tempVel[2] = zV;

		vels.push_back(tempVel);
	}

	cout << "Particles initilized. Beginning simulation...\n";

	//Initialization of cylinders
	vector<Cylinder> cylinders;

	//Single cylinder centered at the origin
	Cylinder c1(1.75, 0.6, 0, 0, 0, 9.4953, 0.027628, settings);
	cylinders.push_back(c1);

	long long graveyardCount = 0;
	long long scatterCount = 0;
	long long absorptionCount = 0;

	for(auto i = 0; i < settings.numParticles; ++i)
	{
		//Print out some pretty stuff for the user
		printInfo(i, settings.numParticles);

		//Spawn a particle
		Particle particle(xPos, yPos, zPos, 
						  vels, settings);

		while(particle.isAlive())
		{
			//Check if particle is in contact with something
			for(auto cyl : cylinders)
			{
				//If the particle is in contact
				if(cyl.isIn(particle))
				{
					//Then calculate what happens
					cyl.Event(particle, rng, rngVel); 
					//As of now, a particle can only be in one cylinder
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

static inline void printInfo(long long ran, 
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