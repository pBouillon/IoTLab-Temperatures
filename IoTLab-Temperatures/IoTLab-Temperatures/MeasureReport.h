#pragma once

using namespace Windows::Data::Json;

class MeasureReport
{
	double battery;

	int brightness;

	double humidity;

	double temperature;

public:

	MeasureReport();

	// Create a new report based on the battery, the brightness,
	// the humidity and the temperature
	MeasureReport(double, int, double, double);

	// Create the report from the IoTLab response
	static MeasureReport FromIotlabResponse(JsonObject^);
};

