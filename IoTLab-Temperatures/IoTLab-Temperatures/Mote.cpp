#include <cmath>

#include "pch.h"
#include "Mote.h"
#include "Degree.cpp"

// Earth radius in km from:
// https://nssdc.gsfc.nasa.gov/planetary/factsheet/earthfact.html
const double EARTH_RADIUS_IN_KM = 6356.752;

Mote::Mote(GeographicCoordinate* coordinate, double name, std::string commonName)
{
	this->commonName = commonName;
	this->coordinate = coordinate;
	this->name = name;
}

Mote::Mote(double latitude, double longitude, double name, std::string commonName)
{
	this->commonName = commonName;
	this->coordinate = &GeographicCoordinate(latitude, longitude);
	this->name = name;
}

// Evaluate the distance between two geographic coordinates in kilometers
// Algorithm from: http://www.movable-type.co.uk/scripts/latlong.html
// And an implementation in JS from: https://stackoverflow.com/a/365853
double Mote::GetDistanceToThisMoteInKm(GeographicCoordinate* coordinate)
{
	double distanceBetweenLatitudes = ToRadians(coordinate->GetLatitude() - this->coordinate->GetLatitude());
	double distanceBetweenLongitudes = ToRadians(coordinate->GetLongitude() - this->coordinate->GetLongitude());

	double latitudeInRadian = ToRadians(this->coordinate->GetLatitude);
	double targetLatitudeInRadian = ToRadians(coordinate->GetLatitude);

	double a = sin(distanceBetweenLatitudes / 2) * sin(distanceBetweenLatitudes / 2)
		+ sin(distanceBetweenLongitudes / 2) * sin(distanceBetweenLongitudes / 2)
		* cos(latitudeInRadian)
		* cos(targetLatitudeInRadian);

	double b = 2 * atan2(sqrt(a), sqrt(1 - a));

	return b * EARTH_RADIUS_IN_KM;
}
