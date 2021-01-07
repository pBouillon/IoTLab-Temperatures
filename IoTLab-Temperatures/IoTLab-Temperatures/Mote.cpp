#include "pch.h"
#include "Mote.h"

Mote::Mote(GeographicCoordinate* coordinate, std::string name, std::string commonName)
{
	this->commonName = commonName;
	this->coordinate = coordinate;
	this->name = name;
}

Mote::Mote(double latitude, double longitude, std::string name, std::string commonName)
{
	this->commonName = commonName;
	this->coordinate = &GeographicCoordinate(latitude, longitude);
	this->name = name;
}

double Mote::GetDistanceToThisMoteInKm(GeographicCoordinate* coordinate)
{
	return this->coordinate->GetDistanceFrom(coordinate);
}
