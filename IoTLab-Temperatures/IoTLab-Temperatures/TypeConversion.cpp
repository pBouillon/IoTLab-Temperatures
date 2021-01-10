#include "pch.h"
#include "TypeConversion.h"

double typeConversion::ToDouble(Platform::String^ value)
{
	std::wstring tmp(value->Begin());

	std::string stringifiedValue(tmp.begin(), tmp.end());

	return atof(stringifiedValue.c_str());
}
