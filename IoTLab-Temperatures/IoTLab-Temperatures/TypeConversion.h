#pragma once

namespace typeConversion
{
	double ToDouble(Platform::String^ value);

	Platform::String^ ToPlatformString(const std::string& input);

	std::string ToString(Platform::String^ value);
}
