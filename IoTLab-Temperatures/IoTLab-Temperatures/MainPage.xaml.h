//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "Mote.h"


namespace IoTLab_Temperatures
{
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		concurrency::cancellation_token_source geopositionTaskTokenSource;

		bool isRealTimeLocationEnabled;

		~MainPage();

		void InitializeThreads();
		
		bool IsLatitudeValid();
		
		bool IsLongitudeValid();

		bool IsUserPositionSet();
		
		void LatitudeBox_TextChanged(
			Platform::Object^ sender,
			Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		
		void LongitudeBox_TextChanged(
			Platform::Object^ sender,
			Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

		void OnTick(Platform::Object^ sender, Platform::Object^ e);

		void RenderDirectionContainer();
		
		void RenderMoteContainer();
		
		void SetBatteryImageFromMeasure(double batteryValue);
		
		void SetBrightnessImageFromMeasure(double brightnessValue);
		
		void SetGeolocationPropertiesText(Platform::String^ latitudeText, Platform::String^ longitudeText);
		
		void SetGeolocationPropertyFromValue(
			Platform::String^ value,
			Windows::UI::Xaml::Controls::ComboBox^ signComboBox,
			Windows::UI::Xaml::Controls::TextBox^ valueTextBox);
		
		void SetHumidityImageFromMeasure(double humidityRate);
		
		void SetTemperatureImageFromMeasure(double temperatureValue);
		
		void ToggleImages(
			Windows::UI::Xaml::Controls::Image^ toActivate,
			Windows::UI::Xaml::Controls::Image^ toDeactivate);
		
		void UpdateBatteryCard(MeasureReport& measure);
		
		void UpdateBrightnessCard(MeasureReport& measure);
		
		void UpdateCards();

		void UpdateDisplay();
		
		void UpdateHumidityCard(MeasureReport& measure);
		
		void UpdateTemperatureCard(MeasureReport& measure);

		void UpdateUserCoordinatesFromFields();

		void UpdateValidateButtonValidity();
		
		void ValidateButton_Click(
			Platform::Object^ sender, 
			Windows::UI::Xaml::RoutedEventArgs^ e);
		
		void LocateButton_Click(
			Platform::Object^ sender, 
			Windows::UI::Xaml::RoutedEventArgs^ e);
		
		void UpdateLocationData(Windows::Devices::Geolocation::Geoposition^ position);
		void CompassImage_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
	};
}
