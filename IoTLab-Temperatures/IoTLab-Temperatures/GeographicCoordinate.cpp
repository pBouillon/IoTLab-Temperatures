#include <cmath>

#include "pch.h"
#include "Degree.h"
#include "GeographicCoordinate.h"

#define DOUBLE_COMPARISON_EPSILON 0.0000001f

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
	{
		throw std::invalid_argument("Incorrect latitude provided");
	}

	this->latitude = latitude;

	if (!IsValidLongitude(longitude))
	{
		throw std::invalid_argument("Incorrect longitude provided");
	}

	this->longitude = longitude;
}

boolean AreDoubleEquals(double x, double y, double epsilon = DOUBLE_COMPARISON_EPSILON)
{
	return fabs(x - y) < epsilon;
}

CardinalPointFlags GeographicCoordinate::GetDirectionTo(GeographicCoordinate& coordinate)
{
	bool isNorthFromCurrentPosition = this->latitude > coordinate.latitude;
		
	bool isEastFromCurrentPosition = this->longitude > coordinate.longitude;

	CardinalPointFlags direction = isNorthFromCurrentPosition
		? NORTH
		: SOUTH;

	direction |= isEastFromCurrentPosition
		? EAST
		: WEST;

	return direction;
}

// Evaluate the distance between two geographic coordinates in kilometers using the Haversine formula
// Algorithm from: http://www.movable-type.co.uk/scripts/latlong.html
// And an implementation in JS from: https://stackoverflow.com/a/365853
double GeographicCoordinate::GetDistanceFromInKm(GeographicCoordinate& coordinate)
{
	double distanceBetweenLatitudesInRadian = degree::ToRadians(coordinate.GetLatitude() - this->latitude);
	double distanceBetweenLongitudesInRadian = degree::ToRadians(coordinate.GetLongitude() - this->longitude);

	double latitudeInRadian = degree::ToRadians(this->latitude);
	double targetLatitudeInRadian = degree::ToRadians(coordinate.GetLatitude());

	double a =  pow(sin(distanceBetweenLatitudesInRadian / 2), 2)
		+  pow(sin(distanceBetweenLongitudesInRadian / 2), 2)
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

bool GeographicCoordinate::HasSameCoordinateAs(GeographicCoordinate& coordinate)
{
	return HasSameLatitudeAs(coordinate) 
		&& HasSameLongitudeAs(coordinate);
}

bool GeographicCoordinate::HasSameLatitudeAs(GeographicCoordinate& coordinate)
{
	return AreDoubleEquals(this->latitude, coordinate.latitude);
}

bool GeographicCoordinate::HasSameLongitudeAs(GeographicCoordinate& coordinate)
{
	return AreDoubleEquals(this->longitude, coordinate.longitude);
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
