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
		std::vector<Mote*> motes;

		~MainPage();

		bool IsLatitudeValid();
		bool IsLongitudeValid();
		void LatitudeBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void LongitudeBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void RenderClosestMote();
		void RenderClosestMoteBattery();
		void RenderClosestMoteBrightness();
		void RenderClosestMoteHumidity();
		void RenderClosestMoteMeasure();
		void RenderClosestMoteTemperature();
		void RetrieveTemperatureFromIoTLab();
		void SetClosestMoteFromCoordinate(GeographicCoordinate& coordinate);
		void UpdateDisplayedMeasures();
		void UpdateValidateButtonValidity();
		void ValidateButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
