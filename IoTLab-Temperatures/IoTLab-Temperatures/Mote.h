#pragma once

#include <string>

#include "GeographicCoordinate.h"

// Create a new instance of a mote managed by the IoTLab.
// Example:
//     GeographicCoordinate moteCoordinate (49.59933, 6.10567);
//     Mote mote (&moteCoordinate, "151.105", "salle_E-1.22");
// Or:
//     Mote mote1 (49.59933, 6.10567, "151.105", "salle_E-1.22");
class Mote
{
	// Common name of the mote, human readable and human friendly
	std::string commonName;

	// Geocraphic coordinate of the mote
	GeographicCoordinate* coordinate;

	// Raw name and identifier of the mote from the IoTLab
	std::string name;

public:
	// Create a new mote from a geographic coordinate, its identifier and its name
	Mote(GeographicCoordinate*, std::string, std::string);

	// Create a new mote from its coordinates, its identifier and its name
	Mote(double, double, std::string, std::string);

	// Get the distance between this mote and a coordinate in kilometers
	double GetDistanceToThisMoteInKm(GeographicCoordinate*);
};

