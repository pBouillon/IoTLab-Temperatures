#pragma once

namespace typeConversion
{
	double ToDouble(Platform::String^);

	Platform::String^ ToPlatformString(const std::string&);

	std::string ToString(Platform::String^ value);
}
