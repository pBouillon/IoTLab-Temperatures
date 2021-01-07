//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

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
Boolean IsLatitudeValueSet = false;
Boolean IsLongitudeValueSet = false;


MainPage::MainPage()
{
	InitializeComponent();
}


void IoTLab_Temperatures::MainPage::ValidateButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	CoordinatesBox->Text = LatitudeBox->Text + "," + LongitudeBox->Text;
}


void IoTLab_Temperatures::MainPage::LatitudeBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	IsLatitudeValueSet = LatitudeBox->Text->Length() >= 8;
	ValidateButton->IsEnabled = IsLatitudeValueSet && IsLongitudeValueSet;
}


void IoTLab_Temperatures::MainPage::LongitudeBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	IsLongitudeValueSet = LongitudeBox->Text->Length() >= 8;
	ValidateButton->IsEnabled = IsLatitudeValueSet && IsLongitudeValueSet;
}
