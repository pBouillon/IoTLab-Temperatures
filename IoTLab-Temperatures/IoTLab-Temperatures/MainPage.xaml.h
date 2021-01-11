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
		Mote* closestMote;

		~MainPage();

		bool IsLatitudeValid();
		bool IsLongitudeValid();
		void LatitudeBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void LongitudeBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void RetrieveTemperatureFromIoTLab();
		void UpdateValidateButtonValidity();
		void ValidateButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
