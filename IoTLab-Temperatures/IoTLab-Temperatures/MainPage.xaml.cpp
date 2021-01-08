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


const Platform::String^ GEOGRAPHIC_COORDINATE_SEPARATOR = ", ";

const int INPUT_MAX_SIZE = 8;


// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409


MainPage::MainPage()
{
	InitializeComponent();
}


void IoTLab_Temperatures::MainPage::ValidateButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Platform::String^ latitudeSign = LatitudeSignComboBox->SelectedItem->ToString();
	Platform::String^ longitudeSign = LongitudeSignComboBox->SelectedItem->ToString();

	Platform::String^ latitudeValue = LatitudeBox->Text;
	Platform::String^ longitudeValue = LongitudeBox->Text;

	CoordinatesBox->Text = latitudeSign + latitudeValue
		+ GEOGRAPHIC_COORDINATE_SEPARATOR
		+ longitudeSign + longitudeValue;
}


void IoTLab_Temperatures::MainPage::LatitudeBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	SetValidateButtonValidity();
}


void IoTLab_Temperatures::MainPage::LongitudeBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	SetValidateButtonValidity();
}


void IoTLab_Temperatures::MainPage::SetValidateButtonValidity()
{
	ValidateButton->IsEnabled = IsLatitudeValid()
		&& IsLongitudeValid();
}


bool IoTLab_Temperatures::MainPage::IsLatitudeValid() {
	Platform::String^ content = LatitudeBox->Text;

	return IsInputValid(content)
		&& GeographicCoordinate::IsValidLatitude(ToDouble(content));
}


bool IoTLab_Temperatures::MainPage::IsLongitudeValid() {
	Platform::String^ content = LongitudeBox->Text;

	return IsInputValid(content)
		&& GeographicCoordinate::IsValidLongitude(ToDouble(content));
}


bool IoTLab_Temperatures::MainPage::IsInputValid(Platform::String^ input) {
	return input->Length() <= INPUT_MAX_SIZE
		&& input->Length() > 0;
}


double IoTLab_Temperatures::MainPage::ToDouble(Platform::String^ value) {
	std::wstring tmp(value->Begin());

	std::string stringifiedValue(tmp.begin(), tmp.end());
	
	return atof(stringifiedValue.c_str());
}