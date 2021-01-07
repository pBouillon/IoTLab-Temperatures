#include "pch.h"
#include "GeographicCoordinate.h"

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
