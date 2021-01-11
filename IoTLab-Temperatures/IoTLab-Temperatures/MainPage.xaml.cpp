//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "GeographicCoordinate.h"

using namespace IoTLab_Temperatures;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;


// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409


// Separator used when displaying alongside the latitude and the longitude of the user
const Platform::String^ GEOGRAPHIC_COORDINATE_SEPARATOR = ", ";


// TODO: Replace with mote
double battery = 50.0;
double brightness = 210.0;
double humidity = 75.0;
double temperature = 22.0;



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


void IoTLab_Temperatures::MainPage::UpdateDisplayedMeasures(double battery, double brightness, double humidity, double temperature) {

	BatteryValueTextBlock->Text = battery.ToString() + " %";
	BrightnessValueTextBlock->Text = brightness.ToString() + " Lx";
	HumidityValueTextBlock->Text = humidity.ToString() + " %";
	TemperatureValueTextBlock->Text = temperature.ToString() + " °C";
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

	// TODO: replace with mote's measures
	battery += 4;
	brightness += 4;
	humidity += 4;
	temperature += 4;
	UpdateDisplayedMeasures(battery, brightness, humidity, temperature);

	RenderClosestMoteMeasure();
}

