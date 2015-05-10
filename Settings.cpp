#include "Settings.h"

using namespace std;

Settings::Settings()
{
	read();
}

void Settings::read()
{
	//This is ugly as hell, but it works
	//Open settings file
	ifstream file("Settings/settings");

	//Check if file existed
	if(file.fail())
	{
		cout << "Settings file could not be found.\n";
		exit(1);
	}

	//Garbage goes here.
	string garbage;

	//First comment line and empty line
	getline(file, garbage);
	getline(file, garbage);

	//Header for output settings
	getline(file, garbage);
	//Get output filename
	file >> garbage; file >> garbage; //FILENAME = 
	file >> filename;
	getline(file, garbage); // \n char
	getline(file, garbage); // Empty line

	//Header for Universe Settings
	getline(file, garbage);
	file >> garbage; file >> garbage; //NUM_PARTICLES =
	file >> numParticles;
	getline(file, garbage); // \n char
	file >> garbage; file >> garbage; //STEP =
	file >> step;
	getline(file, garbage); // \n char
	file >> garbage; file >> garbage; //SPHERE_RADIUS =
	file >> sphereR;

	return;
}
