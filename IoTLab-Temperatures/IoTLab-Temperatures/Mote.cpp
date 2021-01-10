#include "pch.h"
#include "MeasureReport.h"
#include "Mote.h"

Mote::~Mote()
{
	delete this->measure;
}

Mote::Mote(GeographicCoordinate* coordinate, std::string name, std::string commonName)
{
	this->commonName = commonName;
	this->coordinate = coordinate;
	this->measure = new MeasureReport();
	this->name = name;
}

Mote::Mote(double latitude, double longitude, std::string name, std::string commonName)
	: Mote(&GeographicCoordinate(latitude, longitude), name, commonName) { }

// Generate a default set of motes based on the data provided in the "mote.csv" file
void Mote::GenerateDefaultMoteSet(std::vector<Mote>& result)
{
	result = {
		Mote(48.669422, 6.155112, "9.138", "amphiNord"),
		Mote(48.668837, 6.154990, "111.130", "amphiSud"),
		Mote(48.668922, 6.155363, "151.105", "salle_E - 1.22"),
		Mote(48.669400, 6.155340, "32.131", "salle_N - 0.3"),
		Mote(48.669439, 6.155265, "97.145", "bureau_2.6"),
		Mote(48.669419, 6.155269, "120.99", "bureau_2.7"),
		Mote(48.669394, 6.155287, "200.124", "bureau_2.8"),
		Mote(48.669350, 6.155310, "53.105", "bureau_2.9")
	};
}

double Mote::GetDistanceToThisMoteInKm(GeographicCoordinate& coordinate)
{
	return this->coordinate->GetDistanceFromInKm(coordinate);
}
