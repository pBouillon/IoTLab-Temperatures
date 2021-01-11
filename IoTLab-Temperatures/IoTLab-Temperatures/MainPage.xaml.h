//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace IoTLab_Temperatures
{
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		bool IsLatitudeValid();
		bool IsLongitudeValid();
		void LatitudeBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void LongitudeBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void RenderClosestMote();
		void RenderClosestMoteBattery();
		void RenderClosestMoteBrightness();
		void RenderClosestMoteHumidity();
		void RenderClosestMoteMeasure();
		double ToDouble(Platform::String ^ value);
		void RenderClosestMoteTemperature();
		void UpdateDisplayedMeasures(double battery, double brightness, double humidity, double temperature);
		void UpdateValidateButtonValidity();
		void ValidateButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
