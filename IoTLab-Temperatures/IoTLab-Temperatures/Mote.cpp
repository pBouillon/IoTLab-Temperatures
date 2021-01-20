#include <string>

#include "pch.h"
#include "MeasureReport.h"
#include "Mote.h"
#include "TypeConversion.h"

using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;

// IoTLab API URI
const Platform::String^ IOTLAB_URI = "http://iotlab.telecomnancy.eu:8080/iotlab/rest";

// URI on which the latest measure of the mote will be fetched.
// The mote name should be appended to query only one mote.
const Platform::String^ MOTE_MEASURE_URI = "/data/1/temperature-light2-light1-battery_indicator-humidity/1";

Mote::Mote()
	: Mote(0, 0, "", "") { }

Mote::Mote(GeographicCoordinate* coordinate, std::string name, std::string commonName)
{
	this->commonName = commonName;
	this->coordinate = coordinate;
	this->measure = new MeasureReport();
	this->name = name;
}

Mote::Mote(double latitude, double longitude, std::string name, std::string commonName)
	: Mote(new GeographicCoordinate(latitude, longitude), name, commonName) { }

double Mote::GetDistanceToThisMoteInKm(GeographicCoordinate& coordinate)
{
	return this->coordinate->GetDistanceFromInKm(coordinate);
}

std::string Mote::GetCommonName()
{
	return this->commonName;
}

CardinalPointFlags Mote::GetDirectionToThisMote(GeographicCoordinate& coordinate)
{
	return coordinate.GetDirectionTo(*(this->coordinate));
}

MeasureReport* Mote::GetMeasure()
{
	return this->measure;
}

double Mote::GetLatitude()
{
	return this->coordinate->GetLatitude();
}

double Mote::GetLongitude()
{
	return this->coordinate->GetLongitude();
}

std::string Mote::GetName()
{
	return this->name;
}

bool Mote::HasSameCoordinateAs(GeographicCoordinate& coordinate)
{
	return this->coordinate->HasSameCoordinateAs(coordinate);
}

bool Mote::HasSameLatitudeAs(GeographicCoordinate& coordinate)
{
	return this->coordinate->HasSameLatitudeAs(coordinate);
}

bool Mote::HasSameLongitudeAs(GeographicCoordinate& coordinate)
{
	return this->coordinate->HasSameLongitudeAs(coordinate);
}

void Mote::LoadLatestMeasure()
{
	Platform::String^ measureUri = IOTLAB_URI + MOTE_MEASURE_URI
		+ "/" + typeConversion::ToPlatformString(name);

	Windows::Foundation::Uri^ uri = ref new Uri(measureUri);

	Windows::Web::Http::HttpClient^ httpClient = ref new HttpClient();

	create_task(httpClient->GetStringAsync(uri))
		.then([=](Platform::String^ IoTLabResponse)
	{
		JsonObject^ jsonResponse = JsonObject::Parse(IoTLabResponse);

		delete this->measure;
		this->measure = MeasureReport::FromIotlabResponse(jsonResponse);

		return task_from_result();
	});
}
