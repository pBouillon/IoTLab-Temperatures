#pragma once

#include "CardinalPoint.h"

// Maximum absolute degree of the latitude
const int MAX_ABSOLUTE_LATITUDE_DEGREE = 90;

// Maximum absolute degree of the longitude
const int MAX_ABSOLUTE_LONGITUDE_DEGREE = 180;

// Create a new instance of a geographic coordinate.
// Example:
//     GeographicCoordinate coordinate (49.59933, 6.10567);
class GeographicCoordinate
{
	// Latitude of the geographic coordinate
	double latitude;

	// Longitude of the geographic coordinate
	double longitude;

public:
	// Create a new instance of a geographic coordinate, based on the provided
	// angles
	GeographicCoordinate(double latitude, double longitude);

	// Get the direction to the current coordinate from the provided ones
	CardinalPointFlags GetDirectionTo(GeographicCoordinate& coordinate);

	// Evaluate the distance between two geographic coordinates in kilometers
	double GetDistanceFromInKm(GeographicCoordinate& coordinate);

	// Get the latitude
	double GetLatitude();

	// Get the longitude
	double GetLongitude();

	bool HasSameLatitudeAs(GeographicCoordinate & coordinate);

	bool HasSameLongitudeAs(GeographicCoordinate & coordinate);

	// Check if a latitude is within the allowed bounds of
	// +- MAX_ABSOLUTE_LATITUDE_DEGREE
	static bool IsValidLatitude(double latitude);

	// Check if a longitude is within the allowed bounds of
	// +- MAX_ABSOLUTE_LONGITUDE_DEGREE
	static bool IsValidLongitude(double longitude);
};
