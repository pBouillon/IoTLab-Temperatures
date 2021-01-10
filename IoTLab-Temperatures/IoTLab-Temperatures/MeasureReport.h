#pragma once

using namespace Windows::Data::Json;

// Create an instance of a measure report of a mote which is holding
// the various data it has measured such as the battery in percentages,
// the brightness in lux, the humidity in percentages and the
// temperature in Celsius
class MeasureReport
{
	// Measured battery level, in percentages
	double battery;

	// Measured brightness level, in Lux
	double brightness;

	// Measured humidity level, in percentages 
	double humidity;

	// Measured temperature, in Celsius
	double temperature;

	// Extract the value of a standardize IoTLab JSON row such as:
	// { "timestamp": 1608301045, "label": "battery_indicator", "value": 0.0, "mote": "9.138" }
	static double MeasureReport::ExtractValue(JsonObject^);

	void SetBattery(double);

	void SetBrightness(double);

	void SetHumidity(double);

	void SetTemperature(double);

public:

	// Create an empty measure report with all default values (0)
	MeasureReport();

	// Create a new report based on the battery, the brightness,
	// the humidity and the temperature
	MeasureReport(double, double, double, double);

	// Create the report from the IoTLab response
	static MeasureReport* FromIotlabResponse(JsonObject^);
};

