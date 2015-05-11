#ifndef ISOTOPE_H
#define ISOTOPE_H

#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

class Isotope
{
public:
	Isotope(std::string name_in, 
			int atomicWgt_in, 
			std::string dat_filename);

	//Given an energy, returns the closest* applicable cross section value
	double getCrossSection(double searchEnergy);

private:
	std::string name;
	int atomicWeight;

	//Struct for holding energy v. cross-section data
	struct EnCS
	{
		double energy;
		double crossSection;
	};

	std::vector<EnCS> EnCSVec;

	//Functor used to sort the EnCS vector
	class EnCSSort
	{
	public:
		bool operator() (const EnCS& a, const EnCS& b)
		{
			return a.energy < b.energy;
		}
	};
};

#endif