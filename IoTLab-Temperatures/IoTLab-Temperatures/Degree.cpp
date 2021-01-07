#define _USE_MATH_DEFINES

#include <cmath>

// Convert a value in radians to degrees
double ToDegrees(double radians)
{
	return (radians * 180) / M_PI;
}

// Convert a value in degrees to radians
double ToRadians(double degrees)
{
	return (degrees * M_PI) / 180;
}
