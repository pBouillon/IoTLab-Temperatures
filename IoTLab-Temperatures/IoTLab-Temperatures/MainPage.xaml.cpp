//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#define _CRT_SECURE_NO_DEPRECATE

#include <codecvt>
#include <iterator>
#include <locale> 
#include <shared_mutex>
#include <stdio.h>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "pch.h"
#include "MainPage.xaml.h"
#include "GeographicCoordinate.h"

using namespace IoTLab_Temperatures;

using namespace Concurrency;
using namespace Platform;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Web::Http;


// Separator used when displaying alongside the latitude and the longitude of the user
const Platform::String^ GEOGRAPHIC_COORDINATE_SEPARATOR = ", ";

// IoTLab API URI
const Platform::String^ IOTLAB_URI = "http://iotlab.telecomnancy.eu:8080/iotlab/rest";


MainPage::MainPage()
{
	InitializeComponent();
}


bool IoTLab_Temperatures::MainPage::IsLatitudeValid() {
	Platform::String^ content = LatitudeBox->Text;

	return !content->IsEmpty()
		&& GeographicCoordinate::IsValidLatitude(ToDouble(content));
}


bool IoTLab_Temperatures::MainPage::IsLongitudeValid() {
	Platform::String^ content = LongitudeBox->Text;

	return !content->IsEmpty()
		&& GeographicCoordinate::IsValidLongitude(ToDouble(content));
}


void IoTLab_Temperatures::MainPage::LatitudeBox_TextChanged(
	Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	UpdateValidateButtonValidity();
}


void IoTLab_Temperatures::MainPage::LongitudeBox_TextChanged(
	Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	UpdateValidateButtonValidity();
}


void IoTLab_Temperatures::MainPage::RetrieveTemperatureFromIoTLab()
{
	Platform::String^ url = IOTLAB_URI + "/data/1/temperature-light2-light1-battery_indicator-humidity/1/";
	Windows::Foundation::Uri^ uri = ref new Uri(url);

	Windows::Web::Http::HttpClient^ httpClient = ref new HttpClient();

	create_task(httpClient->GetStringAsync(uri))
		.then([=](Platform::String^ Tet) 
	{
		auto objJson = JsonObject::Parse(Tet);
		
		JsonArray^ data = objJson->GetObject()->GetNamedArray("data")->GetArray();
		
		int jsonArraySize = data->Size;
		
		for (int i = 0; i < jsonArraySize; i++) {
			if (data->GetObjectAt(i)->GetNamedString("label")->Equals("temperature")) {
				Platform::String^ temprature = data->GetObjectAt(i)->GetNamedValue("value")->ToString();
			}
		}

		return task_from_result();
	});
}


double IoTLab_Temperatures::MainPage::ToDouble(Platform::String^ value) {
	std::wstring tmp(value->Begin());

	std::string stringifiedValue(tmp.begin(), tmp.end());
	
	return atof(stringifiedValue.c_str());
}


// Enable the "Validate" button depending of the validity of the other fields
void IoTLab_Temperatures::MainPage::UpdateValidateButtonValidity()
{
	ValidateButton->IsEnabled = IsLatitudeValid()
		&& IsLongitudeValid();
}


// On click, build the user's geographic coordinate from the latitude and the longitude input fields
// and display it
void IoTLab_Temperatures::MainPage::ValidateButton_Click(
	Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Platform::String^ latitudeValue = LatitudeBox->Text;
	Platform::String^ latitudeSign = LatitudeSignComboBox->SelectedItem->ToString();
	Platform::String^ formattedLatitude = latitudeSign + latitudeValue;

	Platform::String^ longitudeValue = LongitudeBox->Text;
	Platform::String^ longitudeSign = LongitudeSignComboBox->SelectedItem->ToString();
	Platform::String^ formattedLongitude = longitudeSign + longitudeValue;

	Platform::String^ userCoordinate = formattedLatitude + GEOGRAPHIC_COORDINATE_SEPARATOR + formattedLongitude;

	CoordinatesBox->Text = userCoordinate;
}
