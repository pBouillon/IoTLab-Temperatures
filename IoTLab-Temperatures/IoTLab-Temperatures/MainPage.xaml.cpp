//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

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
#include "GeographicCoordinate.h"
#include "MainPage.xaml.h"
#include "Mote.h"
#include "TypeConversion.h"

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


MainPage::MainPage()
{
	InitializeComponent();

	// No mote can be defined as the closest on startup since
	// we do not know the current user's position
	closestMote = NULL;

	// Generate the default mote set with whom the app will be working
	Mote::GenerateDefaultMoteSet(motes);
}


MainPage::~MainPage()
{
	delete closestMote;
}


bool IoTLab_Temperatures::MainPage::IsLatitudeValid() {
	Platform::String^ content = LatitudeBox->Text;

	return !content->IsEmpty()
		&& GeographicCoordinate::IsValidLatitude(typeConversion::ToDouble(content));
}


bool IoTLab_Temperatures::MainPage::IsLongitudeValid() {
	Platform::String^ content = LongitudeBox->Text;

	return !content->IsEmpty()
		&& GeographicCoordinate::IsValidLongitude(typeConversion::ToDouble(content));
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
	closestMote->LoadLatestMeasure();
}


void IoTLab_Temperatures::MainPage::SetClosestMoteFromCoordinate(GeographicCoordinate& coordinate) {
	double shortestDistance = INT_MAX;

	for (unsigned int i = 0; i < motes.size(); ++i) {
		Mote current = motes[i];
		double distance = current.GetDistanceToThisMoteInKm(coordinate);

		if (distance < shortestDistance) {
			closestMote = &motes[i];
		}
	}
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

	Platform::String^ displayedUserCoordinate = formattedLatitude + GEOGRAPHIC_COORDINATE_SEPARATOR + formattedLongitude;

	CoordinatesBox->Text = displayedUserCoordinate;

	GeographicCoordinate userCoordinate (
		typeConversion::ToDouble(formattedLatitude), typeConversion::ToDouble(formattedLongitude));

	SetClosestMoteFromCoordinate(userCoordinate);

	RetrieveTemperatureFromIoTLab();
}
