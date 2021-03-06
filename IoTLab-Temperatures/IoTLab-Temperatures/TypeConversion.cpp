#include "pch.h"
#include "TypeConversion.h"

#include <iomanip>
#include <sstream>

namespace typeConversion
{
	double ToDouble(Platform::String^ value)
	{
		return atof(ToString(value).c_str());
	}

	Platform::String^ ToPlatformString(const std::string& input)
	{
		std::wstring w_str = std::wstring(input.begin(), input.end());
		const wchar_t* w_chars = w_str.c_str();

		return (ref new Platform::String(w_chars));
	}

	Platform::String^ ToPlatformStringWithPrecision(double value, unsigned int precision)
	{
		std::stringstream ss;

		ss << std::fixed 
			<< std::setprecision(precision)
			<< value;

		return ToPlatformString(ss.str());
	}

	std::string ToString(Platform::String^ value)
	{
		std::wstring tmp(value->Begin());

		return std::string(tmp.begin(), tmp.end());
	}
}
