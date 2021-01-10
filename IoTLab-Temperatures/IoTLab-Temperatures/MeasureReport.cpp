#include "pch.h"
#include "MeasureReport.h"


MeasureReport::MeasureReport()
	: MeasureReport(0, 0, 0, 0) { }


MeasureReport::MeasureReport(
	double battery, int brightness, double humidity, double temperature)
{
	this->battery = battery;
	this->brightness = brightness;
	this->humidity = humidity;
	this->temperature = temperature;
}


// TODO: read JSON
MeasureReport MeasureReport::FromIotlabResponse(JsonObject ^)
{
	return MeasureReport(0, 0, 0, 0);
}
