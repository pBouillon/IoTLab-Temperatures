#include "pch.h"
#include "MeasureReport.h"
#include "TypeConversion.h"

using namespace Windows::Data::Json;

// JSON key to access the array of records
// Note:
//     This can't be marked as const since it is used in GetNamedArray()
//     which allows only 'Platform::String^' and not 'const Platform::String^'
Platform::String^ IOTLAB_JSON_DATA_KEY = "data";

// JSON key to characterizing a JSON record as holding the battery level measured by the mote
// Note:
//     This can't be marked as const since it is used in GetNamedValue()
//     which allows only 'Platform::String^' and not 'const Platform::String^'
Platform::String^ IOTLAB_JSON_BATTERY_KEY = "battery";

// JSON key to characterizing a JSON record as holding the brightness measured by the mote
// Note:
//     This can't be marked as const since it is used in GetNamedValue()
//     which allows only 'Platform::String^' and not 'const Platform::String^'
Platform::String^ IOTLAB_JSON_BRIGHTNESS_KEY = "light1";

// JSON key to characterizing a JSON record as holding the humidity level measured by the mote
// Note:
//     This can't be marked as const since it is used in GetNamedValue()
//     which allows only 'Platform::String^' and not 'const Platform::String^'
Platform::String^ IOTLAB_JSON_HUMIDITY_KEY = "humidity";

// JSON key to retrieve the label of a JSON record
// Note:
//     This can't be marked as const since it is used in GetNamedValue()
//     which allows only 'Platform::String^' and not 'const Platform::String^'
Platform::String^ IOTLAB_JSON_LABEL_KEY = "label";

// JSON key to characterizing a JSON record as holding the temperature measured by the mote
// Note:
//     This can't be marked as const since it is used in GetNamedValue()
//     which allows only 'Platform::String^' and not 'const Platform::String^'
Platform::String^ IOTLAB_JSON_TEMPERATURE_KEY = "temperature";

// JSON key to access the value of a record
// Note:
//     This can't be marked as const since it is used in GetNamedValue()
//     which allows only 'Platform::String^' and not 'const Platform::String^'
Platform::String^ IOTLAB_JSON_VALUE_KEY = "value";


MeasureReport::MeasureReport()
	: MeasureReport(0, 0, 0, 0) { }


MeasureReport::MeasureReport(
	double battery, double brightness, double humidity, double temperature)
{
	this->battery = battery;
	this->brightness = brightness;
	this->humidity = humidity;
	this->temperature = temperature;
}


double MeasureReport::ExtractValue(JsonObject^ json)
{
	Platform::String^ rawValue = json->GetNamedValue(IOTLAB_JSON_VALUE_KEY)
		->ToString();

	return typeConversion::ToDouble(rawValue);
}


MeasureReport* MeasureReport::FromIotlabResponse(JsonObject^ json)
{
	// Create a hollow report to be populated as the JSON is processed
	MeasureReport* report = new MeasureReport();

	JsonArray^ measures = json->GetObject()
		->GetNamedArray(IOTLAB_JSON_DATA_KEY)
		->GetArray();

	// Iterate on the measures and extract their values to populate the created report
	for (int i = 0; i < measures->Size; ++i) {
		JsonObject^ row = measures->GetObjectAt(i);

		double measure = ExtractValue(row);
		Platform::String^ measureType = row->GetNamedString(IOTLAB_JSON_LABEL_KEY);

		if (measureType->Equals(IOTLAB_JSON_BATTERY_KEY)) {
			report->SetBattery(measure);
		}

		else if (measureType->Equals(IOTLAB_JSON_BRIGHTNESS_KEY)) {
			report->SetBrightness(measure);
		}

		else if (measureType->Equals(IOTLAB_JSON_HUMIDITY_KEY)) {
			report->SetHumidity(measure);
		}

		else if (measureType->Equals(IOTLAB_JSON_TEMPERATURE_KEY)) {
			report->SetTemperature(measure);
		}
	}

	return report;
}


double MeasureReport::GetBattery()
{
	return battery;
}


double MeasureReport::GetBrightness()
{
	return brightness;
}


double MeasureReport::GetHumidity()
{
	return humidity;
}


double MeasureReport::GetTemperature()
{
	return temperature;
}


void MeasureReport::SetBattery(double battery)
{
	this->battery = battery;
}


void MeasureReport::SetBrightness(double brightness)
{
	this->brightness = brightness;
}


void MeasureReport::SetHumidity(double humidity)
{
	this->humidity = humidity;
}


void MeasureReport::SetTemperature(double temperature)
{
	this->temperature = temperature;
}
