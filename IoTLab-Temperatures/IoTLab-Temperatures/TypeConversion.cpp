#include "pch.h"
#include "TypeConversion.h"

namespace typeConversion
{
	double ToDouble(Platform::String^ value)
	{
		std::wstring tmp(value->Begin());

		std::string stringifiedValue(tmp.begin(), tmp.end());

		return atof(stringifiedValue.c_str());
	}

	Platform::String^ ToPlatformString(const std::string& input)
	{
		std::wstring w_str = std::wstring(input.begin(), input.end());
		const wchar_t* w_chars = w_str.c_str();

		return (ref new Platform::String(w_chars));
	}
}
