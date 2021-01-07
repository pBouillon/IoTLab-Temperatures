#include <cmath>

#include "pch.h"
#include "Degree.h"
#include "GeographicCoordinate.h"

// Earth radius in km from:
// https://nssdc.gsfc.nasa.gov/planetary/factsheet/earthfact.html
const double EARTH_RADIUS_IN_KM = 6356.752;

// Create a new instance of a geographic coordinate, based on the provided
// angles. If any of the provided angles is an illegal value, throws an
// invalid_argument.
// For the validity of the coordinates, refer to:
// https://en.wikipedia.org/wiki/Geographic_coordinate_system#Latitude_and_longitude
GeographicCoordinate::GeographicCoordinate(double latitude, double longitude)
{
	if (!IsValidLatitude(latitude))
		throw std::invalid_argument("Incorrect latitude provided");

	this->latitude = latitude;

	if (!IsValidLongitude(longitude))
		throw std::invalid_argument("Incorrect longitude provided");

	this->longitude = longitude;
}

// Evaluate the distance between two geographic coordinates in kilometers
// Algorithm from: http://www.movable-type.co.uk/scripts/latlong.html
// And an implementation in JS from: https://stackoverflow.com/a/365853
double GeographicCoordinate::GetDistanceFrom(GeographicCoordinate& coordinate)
{
	double distanceBetweenLatitudes = degree::ToRadians(coordinate.GetLatitude() - this->latitude);
	double distanceBetweenLongitudes = degree::ToRadians(coordinate.GetLongitude() - this->longitude);

	double latitudeInRadian = degree::ToRadians(this->latitude);
	double targetLatitudeInRadian = degree::ToRadians(coordinate.GetLatitude());

	double a = sin(distanceBetweenLatitudes / 2) * sin(distanceBetweenLatitudes / 2)
		+ sin(distanceBetweenLongitudes / 2) * sin(distanceBetweenLongitudes / 2)
		* cos(latitudeInRadian)
		* cos(targetLatitudeInRadian);

	double b = 2 * atan2(sqrt(a), sqrt(1 - a));

	return b * EARTH_RADIUS_IN_KM;
}

double GeographicCoordinate::GetLatitude()
{
	return this->latitude;
}

double GeographicCoordinate::GetLongitude()
{
	return this->longitude;
}

bool GeographicCoordinate::IsValidLatitude(double latitude)
{
	return -MAX_ABSOLUTE_LATITUDE_DEGREE <= latitude
		&& latitude <= MAX_ABSOLUTE_LATITUDE_DEGREE;
}

bool GeographicCoordinate::IsValidLongitude(double longitude)
{
	return -MAX_ABSOLUTE_LONGITUDE_DEGREE <= longitude
		&& longitude <= MAX_ABSOLUTE_LONGITUDE_DEGREE;
}
