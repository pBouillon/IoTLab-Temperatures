#pragma once

namespace typeConversion
{
	double ToDouble(Platform::String^ value);

	Platform::String^ ToPlatformString(const std::string& input);
	
	Platform::String^ ToPlatformStringWithPrecision(double value, unsigned int precision = 2);

	std::string ToString(Platform::String^ value);
}
