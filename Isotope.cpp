#include "Isotope.h"

using namespace std;

Isotope::Isotope(string name_in, 
				 int atomicWgt_in, 
				 string dat_filename, 
				 double scatterCrossSection_in)
:name(name_in), atomicWeight(atomicWgt_in), scatterCrossSection(scatterCrossSection_in)
{
	//Open .dat file and extract info
	ifstream dat(("DATs/" + dat_filename).c_str());
	double energy;
	double crossSection;

	dat >> energy >> crossSection;
	while(!dat.fail())
	{
		EnCS newEnCS = {energy, crossSection};
		EnCSVec.push_back(newEnCS);
		dat >> energy >> crossSection;
	}

	//Sort the vector by energy just in case it isn't sorted beforehand.
	sort(EnCSVec.begin(), EnCSVec.end(), Isotope::EnCSSort());
}

double Isotope::getCrossSection(double searchEnergy)
{
	//Make a dummy EnCS for search purposes
	EnCS search = {searchEnergy, 0};

	//Find the closest* known energy to our search energy
	//*Finds the first energy that is greater than searchEnergy
	auto closest = lower_bound(EnCSVec.begin(), EnCSVec.end(), 
							   search, Isotope::EnCSSort());

	//Edge case if search is the greatest and there is no greater
	if(closest == EnCSVec.end())
		return EnCSVec.back().crossSection;

	return (*closest).crossSection;
}

//for now this only returns a single value given by the user
//Eventually, it will be energy dependent like absorbtion
double Isotope::getScatterCrossSection()
{
	return scatterCrossSection;
}