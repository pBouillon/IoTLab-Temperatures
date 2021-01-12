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
	InitializeMotes();
}


MainPage::~MainPage()
{
	delete closestMote;
}


void IoTLab_Temperatures::MainPage::InitializeMotes()
{
	motes = {
		new Mote(48.669422, 6.155112, "9.138", "amphiNord"),
		new Mote(48.668837, 6.154990, "111.130", "amphiSud"),
		new Mote(48.668922, 6.155363, "151.105", "salle_E - 1.22"),
		new Mote(48.669400, 6.155340, "32.131", "salle_N - 0.3"),
		new Mote(48.669439, 6.155265, "97.145", "bureau_2.6"),
		new Mote(48.669419, 6.155269, "120.99", "bureau_2.7"),
		new Mote(48.669394, 6.155287, "200.124", "bureau_2.8"),
		new Mote(48.669350, 6.155310, "53.105", "bureau_2.9")
	};

	for (unsigned int i = 0; i < motes.size(); ++i)
	{
		motes[i]->LoadLatestMeasure();
	}
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


void IoTLab_Temperatures::MainPage::RenderClosestMote() {
	MoteImage->Visibility = Windows::UI::Xaml::Visibility::Visible;
	MoteLocationTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
	MoteTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
}


void IoTLab_Temperatures::MainPage::RenderClosestMoteBattery() {
	BatteryImage->Visibility = Windows::UI::Xaml::Visibility::Visible;
	BatteryTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
	BatteryRectangle->Visibility = Windows::UI::Xaml::Visibility::Visible;
	BatteryValueTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
}


void IoTLab_Temperatures::MainPage::RenderClosestMoteBrightness() {
	BrightnessImage->Visibility = Windows::UI::Xaml::Visibility::Visible;
	BrightnessTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
	BrightnessRectangle->Visibility = Windows::UI::Xaml::Visibility::Visible;
	BrightnessValueTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
}


void IoTLab_Temperatures::MainPage::RenderClosestMoteHumidity() {
	HumidityImage->Visibility = Windows::UI::Xaml::Visibility::Visible;
	HumidityTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
	HumidityRectangle->Visibility = Windows::UI::Xaml::Visibility::Visible;
	HumidityValueTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
}


void IoTLab_Temperatures::MainPage::RenderClosestMoteMeasure() {
	// Collapse the default text when no mote's measure is displayed
	NoMoteDisplayedTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

	// Update the mote's label
	Platform::String^ moteName = typeConversion::ToPlatformString(closestMote->GetName());
	MoteTextBlock->Text = moteName;

	Platform::String^ moteCommonName = typeConversion::ToPlatformString(closestMote->GetCommonName());
	MoteLocationTextBlock->Text = moteCommonName;

	// Display Mote information (name and location) & measures (battery, brightness, humidity and temperature)
	RenderClosestMote();
	RenderClosestMoteBattery();
	RenderClosestMoteBrightness();
	RenderClosestMoteHumidity();
	RenderClosestMoteTemperature();
}


void IoTLab_Temperatures::MainPage::RenderClosestMoteTemperature() {
	TemperatureImage->Visibility = Windows::UI::Xaml::Visibility::Visible;
	TemperatureTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
	TemperatureRectangle->Visibility = Windows::UI::Xaml::Visibility::Visible;
	TemperatureValueTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
}


void IoTLab_Temperatures::MainPage::RetrieveTemperatureFromIoTLab()
{
	closestMote->LoadLatestMeasure();
}


void IoTLab_Temperatures::MainPage::SetClosestMoteFromCoordinate(GeographicCoordinate& coordinate) {
	double shortestDistance = INT_MAX;

	for (unsigned int i = 0; i < motes.size(); ++i) {
		Mote* current = motes[i];
		double distance = current->GetDistanceToThisMoteInKm(coordinate);

		if (distance < shortestDistance) {
			closestMote = current;
			shortestDistance = distance;
		}
	}
}

// Enable the "Validate" button depending of the validity of the other fields
void IoTLab_Temperatures::MainPage::UpdateValidateButtonValidity()
{
	ValidateButton->IsEnabled = IsLatitudeValid()
		&& IsLongitudeValid();
}


void IoTLab_Temperatures::MainPage::UpdateDisplayedMeasures() {
	MeasureReport* measure = this->closestMote->GetMeasure();

	BatteryValueTextBlock->Text = measure->GetBattery().ToString() + " %";
	
	BrightnessValueTextBlock->Text = measure->GetBrightness().ToString() + " Lx";
	
	HumidityValueTextBlock->Text = measure->GetHumidity().ToString() + " %";

	TemperatureValueTextBlock->Text = measure->GetTemperature().ToString() + " °C";
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

	GeographicCoordinate userCoordinate (
		typeConversion::ToDouble(formattedLatitude), typeConversion::ToDouble(formattedLongitude));

	SetClosestMoteFromCoordinate(userCoordinate);

	RetrieveTemperatureFromIoTLab();

	UpdateDisplayedMeasures();

	RenderClosestMoteMeasure();
}

