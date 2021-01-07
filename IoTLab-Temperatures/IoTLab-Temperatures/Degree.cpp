#include "pch.h"

#include "Degree.h"

namespace degree
{
	constexpr double pi = 3.14159265358979323846;

	double ToDegrees(double radians)
	{
		return (radians * 180) / pi;
	}

	double ToRadians(double degrees)
	{
		return (degrees * pi) / 180;
	}
}
