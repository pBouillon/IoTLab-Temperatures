#pragma once

namespace typeConversion
{
	double ToDouble(Platform::String^);

	Platform::String^ ToPlatformString(const std::string&);
}
