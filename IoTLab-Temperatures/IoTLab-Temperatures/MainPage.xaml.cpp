//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"

#include <codecvt>
#include <iterator>
#include <locale> 
#include <shared_mutex>
#include <stdio.h>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "CardinalPoint.h"
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
using namespace Windows::Devices::Geolocation;
using namespace Windows::Web::Http;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace concurrency;

// Number of milliseconds during which the thread will halted
// while looping when awaiting an incoming event
#define THREAD_HALT_MS 100

// Number of ticks elapsed in a second
// https://docs.microsoft.com/fr-fr/windows/uwp/threading-async/use-a-timer-to-submit-a-work-item
#define TICKS_PER_SECOND 10000000

// Separator used when displaying alongside the latitude and the longitude of the user
const Platform::String^ GEOGRAPHIC_COORDINATE_SEPARATOR = ", ";

// Threshold value used for updating battery image based on mote's measure.
// Below this threshold, the battery is shown as almost empty and above it's shown as almost filled
const double BATTERY_THRESHOLD = 25.0;

// Threshold value used for updating brightess image based on mote's measure
// Below this threshold, the brightness is shown as a turned off light and above it's shown as a turned on light 
const double BRIGHTNESS_THRESHOLD = 200.0;

// Threshold value used for updating humidity image based on mote's measure
// Below the low humidity threshold, the humidity is shown as a single droplet
// Between the low humidity threshold and the medium humidity threshold, the humidity is shown as two droplets
// Above the medium humidity threshold, it is shown as three droplets
const double LOW_HUMIDITY_THRESHOLD = 33.0;
const double MEDIUM_HUMIDITY_THRESHOLD = 66.0;

// Threshold value used for updating temperature image based on mote's measure
// Below this threshold, the temperature is shown as a cold thermometer and above it's shown as a warm thermometer
const double TEMPERATURE_THRESHOLD = 20.0;

// The mote that is the closest of the user's according to his
// coordinates
Mote* closestMote;

// A list containing all the motes used for the application and on the IoTLab
std::vector<Mote*> motes = 
{
		new Mote(48.669422, 6.155112, "9.138", "North Amphitheater"),
		new Mote(48.668837, 6.154990, "111.130", "South Amphitheater"),
		new Mote(48.668922, 6.155363, "151.105", "Room E - 1.22"),
		new Mote(48.669400, 6.155340, "32.131", "Room N - 0.3"),
		new Mote(48.669439, 6.155265, "97.145", "Office 2.6"),
		new Mote(48.669419, 6.155269, "120.99", "Office 2.7"),
		new Mote(48.669394, 6.155287, "200.124", "Office 2.8"),
		new Mote(48.669350, 6.155310, "53.105", "Office 2.9")
};

// The user coordinates, by default (0, 0)
GeographicCoordinate userCoordinate = GeographicCoordinate(0, 0);

void SetClosestMoteFromCoordinate(GeographicCoordinate& coordinate);

HANDLE hUpdateClosestMoteEvent;
DWORD WINAPI UpdateClosestMoteRoutine(LPVOID hEvent);

HANDLE hUpdateMoteMeasureReportEvent;
DWORD WINAPI UpdateMoteMeasureReportRoutine(LPVOID hEvent);

// Mutex to prevent an incoherent behaviour if several update requests of the closest
// mote are sent
std::shared_mutex closestMoteUpdateMutex;

// Mutex preventing the HTTP call to be made before the closest mote is adequately set
std::shared_mutex iotlabHttpCallMutex;

MainPage::MainPage()
{
	InitializeComponent();

	// No mote can be defined as the closest on startup since
	// we do not know the current user's position
	closestMote = NULL;

	// We initialize the motes with their current measure on startup
	for (unsigned int i = 0; i < motes.size(); ++i)
	{
		motes[i]->LoadLatestMeasure();
	}

	// Initialize the background tasks
	InitializeThreads();
}

MainPage::~MainPage()
{
	delete closestMote;
}

void IoTLab_Temperatures::MainPage::InitializeThreads()
{
	// Lock the mutex by default in order to let the thread handling the closest mote
	// handle the workflow
	iotlabHttpCallMutex.lock_shared();

	DWORD threadId;
	
	// Initialize the thread in charge of retrieving the closest mote
	hUpdateClosestMoteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	CreateThread(NULL, 0, UpdateClosestMoteRoutine, (LPVOID) hUpdateClosestMoteEvent, 0, &threadId);

	// Initialize the thread in charge of retrieving the latest measure of a mote
	hUpdateMoteMeasureReportEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	CreateThread(NULL, 0, UpdateMoteMeasureReportRoutine, (LPVOID) hUpdateMoteMeasureReportEvent, 0, &threadId);

	// Initialize the refresh period so that the UI is refreshed every 0.5 second
	TimeSpan delay;
	delay.Duration = 0.5 * TICKS_PER_SECOND;

	// Initialize the refresh timer
	DispatcherTimer^ timer = ref new DispatcherTimer();
	timer->Interval = delay;
	timer->Start();
	timer->Tick += ref new EventHandler<Object^>(this, &MainPage::OnTick);
}

bool IoTLab_Temperatures::MainPage::IsLatitudeValid() 
{
	Platform::String^ content = LatitudeBox->Text;

	return !content->IsEmpty()
		&& GeographicCoordinate::IsValidLatitude(typeConversion::ToDouble(content));
}

bool IoTLab_Temperatures::MainPage::IsLongitudeValid() 
{
	Platform::String^ content = LongitudeBox->Text;

	return !content->IsEmpty()
		&& GeographicCoordinate::IsValidLongitude(typeConversion::ToDouble(content));
}

bool IoTLab_Temperatures::MainPage::IsUserPositionSet()
{
	return closestMote != NULL;
}

void IoTLab_Temperatures::MainPage::LatitudeBox_TextChanged(
	Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	UpdateValidateButtonValidity();
}

void IoTLab_Temperatures::MainPage::LocateButton_Click(
	Platform::Object^ sender,
	Windows::UI::Xaml::RoutedEventArgs^ e)
{
	try
	{
		task<GeolocationAccessStatus> geolocationAccessRequestTask(Windows::Devices::Geolocation::Geolocator::RequestAccessAsync());
		geolocationAccessRequestTask.then([this](task<GeolocationAccessStatus> accessStatusTask)
		{
			// Get will throw an exception if the task was canceled or failed with an error
			auto accessStatus = accessStatusTask.get();

			if (accessStatus != GeolocationAccessStatus::Allowed) {
				SetGeolocationPropertiesText("Access denied", "");
				return;
			}

			auto geolocator = ref new Windows::Devices::Geolocation::Geolocator();

			task<Geoposition^> geopositionTask(geolocator->GetGeopositionAsync(), geopositionTaskTokenSource.get_token());
			geopositionTask.then([this](task<Geoposition^> getPosTask)
			{
				// Get will throw an exception if the task was canceled or failed with an error
				UpdateLocationData(getPosTask.get());
			});
		});
	}
	catch (task_canceled&) { /* Silenced */ }
	catch (Exception^ ex)
	{
		SetGeolocationPropertiesText("Failed to fetch", "");
	}
}

void IoTLab_Temperatures::MainPage::LongitudeBox_TextChanged(
	Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	UpdateValidateButtonValidity();
}

void IoTLab_Temperatures::MainPage::OnTick(Platform::Object^ sender, Platform::Object^ e)
{
	if (IsUserPositionSet())
	{
		UpdateDisplay();
	}
}

void IoTLab_Temperatures::MainPage::RenderDirectionContainer()
{
	// Display the container containing the direction information toward the mote
	MoteDirectionGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;

	// Display the distance to the mote, in km
	DirectionDistanceTextBlock->Text = closestMote->GetDistanceToThisMoteInKm(userCoordinate) + " km";

	// If the user coordinates are on the same place as the mote, their is no direction to indicate
	if (closestMote->HasSameCoordinateAs(userCoordinate))
	{
		DirectionValueTextBlock->Text = "On your position";
		return;
	}

	// Build the direction indication
	CardinalPointFlags directionToMote = closestMote->GetDirectionToThisMote(userCoordinate);
	
	Platform::String^ directionIndication = "";
	Platform::String^ spacing = "";

	// If the user has not the same latitude as the mote, we guide him to the North or the South
	if (!closestMote->HasSameLatitudeAs(userCoordinate))
	{
		directionIndication += directionToMote & NORTH
			? "NORTH"
			: "SOUTH";

		// Since the indication contains a direction regarding the latitude, we append a space
		// if an indication containing the longitude is also appended
		spacing = " ";
	}

	// If the user has not the same longitude as the mote, we guide him to the East or the West
	if (!closestMote->HasSameLongitudeAs(userCoordinate))
	{
		directionIndication += directionToMote & EAST
			? spacing + "EAST"
			: spacing + "WEST";
	}
	
	DirectionValueTextBlock->Text = directionIndication;
}

void IoTLab_Temperatures::MainPage::RenderMoteContainer() 
{
	// Collapse the default text when a mote's measure is displayed
	NoMoteDisplayedTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

	// Update the mote's label
	Platform::String^ moteName = typeConversion::ToPlatformString(closestMote->GetName());
	MoteTextBlock->Text = moteName;

	Platform::String^ moteCommonName = typeConversion::ToPlatformString(closestMote->GetCommonName());
	MoteLocationTextBlock->Text = moteCommonName;

	// Display the mote's measure cards
	MoteMeasureGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void IoTLab_Temperatures::MainPage::SetBatteryImageFromMeasure(double batteryValue) 
{
	batteryValue >= BATTERY_THRESHOLD
		? ToggleImages(MediumBatteryImage, LowBatteryImage)
		: ToggleImages(LowBatteryImage, MediumBatteryImage);
}

void IoTLab_Temperatures::MainPage::SetBrightnessImageFromMeasure(double brightnessValue) 
{
	brightnessValue >= BRIGHTNESS_THRESHOLD
		? ToggleImages(MediumBrightnessImage, LowBrightnessImage)
		: ToggleImages(LowBrightnessImage, MediumBrightnessImage);
}

void SetClosestMoteFromCoordinate(GeographicCoordinate& coordinate) 
{
	double shortestDistance = INT_MAX;

	for (unsigned int i = 0; i < motes.size(); ++i) 
	{
		Mote* current = motes[i];
		double distance = current->GetDistanceToThisMoteInKm(coordinate);

		if (distance < shortestDistance)
		{
			closestMote = current;
			shortestDistance = distance;
		}
	}
}

void IoTLab_Temperatures::MainPage::SetGeolocationPropertiesText(
	Platform::String^ latitudeText, 
	Platform::String^ longitudeText) 
{
	SetGeolocationPropertyFromValue(latitudeText, LatitudeSignComboBox, LatitudeBox);
	SetGeolocationPropertyFromValue(longitudeText, LongitudeSignComboBox, LongitudeBox);
}

void IoTLab_Temperatures::MainPage::SetGeolocationPropertyFromValue(
	Platform::String^ value,
	Windows::UI::Xaml::Controls::ComboBox^ signComboBox,
	Windows::UI::Xaml::Controls::TextBox^ valueTextBox)
{
	std::string longitude = typeConversion::ToString(value);

	if (!value->IsEmpty()
		&& longitude[0] == '-')
	{
		longitude = longitude.substr(1, longitude.length() - 1);
		signComboBox->SelectedIndex = 1;
	}

	valueTextBox->Text = typeConversion::ToPlatformString(longitude);
}

void IoTLab_Temperatures::MainPage::SetHumidityImageFromMeasure(double humidityRate) 
{
	LowHumidityImage->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	MediumHumidityImage->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	HighHumidityImage->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	
	if (humidityRate < LOW_HUMIDITY_THRESHOLD) 
	{
		LowHumidityImage->Visibility = Windows::UI::Xaml::Visibility::Visible;
		return;
	}

	if (humidityRate < MEDIUM_HUMIDITY_THRESHOLD) 
	{
		MediumHumidityImage->Visibility = Windows::UI::Xaml::Visibility::Visible;
		return;
	}

	HighHumidityImage->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void IoTLab_Temperatures::MainPage::SetTemperatureImageFromMeasure(double temperatureValue) 
{
	temperatureValue >= TEMPERATURE_THRESHOLD
		? ToggleImages(WarmTemperatureImage, ColdTemperatureImage)
		: ToggleImages(ColdTemperatureImage, WarmTemperatureImage);
}

void IoTLab_Temperatures::MainPage::ToggleImages(
	Windows::UI::Xaml::Controls::Image^ toActivate,
	Windows::UI::Xaml::Controls::Image^ toDeactivate) 
{
	toActivate->Visibility = Windows::UI::Xaml::Visibility::Visible;
	toDeactivate->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void IoTLab_Temperatures::MainPage::UpdateBatteryCard(MeasureReport& measure) 
{
	double batteryValue = measure.GetBattery();

	// Update the battery label
	BatteryValueTextBlock->Text = batteryValue.ToString() + " %";

	// Update the associated image
	SetBatteryImageFromMeasure(batteryValue);
}

void IoTLab_Temperatures::MainPage::UpdateBrightnessCard(MeasureReport& measure) 
{
	double brightnessValue = measure.GetBrightness();

	// Update the brightness label
	BrightnessValueTextBlock->Text = brightnessValue.ToString() + " Lx";

	// Update the associated image
	SetBrightnessImageFromMeasure(brightnessValue);
}

void IoTLab_Temperatures::MainPage::UpdateCards() 
{
	MeasureReport* measure = closestMote->GetMeasure();

	UpdateBatteryCard(*measure);
	UpdateBrightnessCard(*measure);
	UpdateHumidityCard(*measure);
	UpdateTemperatureCard(*measure);
}

DWORD WINAPI UpdateClosestMoteRoutine(LPVOID hEvent)
{
	DWORD dwWait;

	// Wait indefinitely for an incoming event
	for (;;)
	{
		Sleep(THREAD_HALT_MS);
		dwWait = WaitForSingleObject(hEvent, INFINITE);

		// If the incoming event is the one the thread is not the one requesting
		// to find the closest mote, we discard the request and continue
		// to await for the one we are expecting
		if (dwWait != WAIT_OBJECT_0)
		{
			continue;
		}

		// Prevent this function to be executed if another update is requested
		closestMoteUpdateMutex.lock_shared();

		SetClosestMoteFromCoordinate(userCoordinate);

		// Once that the closest mote is found, we can release the mutex
		// so that the other thread can perform the HTTP call
		iotlabHttpCallMutex.unlock_shared();

		// Lock the mutex to prevent the other thread to perform other API calls
		iotlabHttpCallMutex.lock_shared();

		// Once the event is handled, we can clear it
		ResetEvent(hUpdateMoteMeasureReportEvent);

		// Allows for a new update to be performed by releasing the mutex
		closestMoteUpdateMutex.unlock_shared();
	}

	return 0;
}

void IoTLab_Temperatures::MainPage::UpdateDisplay() 
{
	RenderMoteContainer();
	RenderDirectionContainer();
	UpdateCards();
}

void IoTLab_Temperatures::MainPage::UpdateHumidityCard(MeasureReport& measure) 
{
	double humidityRate = measure.GetHumidity();

	// Update the humidity label
	HumidityValueTextBlock->Text = humidityRate.ToString() + " %";

	// Update the associated image
	SetHumidityImageFromMeasure(humidityRate);
}

DWORD WINAPI UpdateMoteMeasureReportRoutine(LPVOID hEvent)
{
	DWORD dwWait;

	// Wait indefinitely for an incoming event
	for (;;)
	{
		Sleep(THREAD_HALT_MS);
		dwWait = WaitForSingleObject(hEvent, INFINITE);

		// If the incoming event is the one the thread is not the one requesting
		// the closest mote's newest measure, we discard the request and continue
		// to await for the one we are expecting
		if (dwWait != WAIT_OBJECT_0)
		{
			continue;
		}

		// Lock the mutex to perform the http call based on the closest mote
		iotlabHttpCallMutex.lock_shared();

		closestMote->LoadLatestMeasure();

		// Release the mutex once the call has been made so that the closest mote
		// can be computed again
		iotlabHttpCallMutex.unlock_shared();

		// Once the event is handled, we can clear it
		ResetEvent(hUpdateMoteMeasureReportEvent);
	}

	return 0;
}

void IoTLab_Temperatures::MainPage::UpdateTemperatureCard(MeasureReport& measure) 
{
	double temperatureValue = measure.GetTemperature();

	// Update the temperature label
	TemperatureValueTextBlock->Text = temperatureValue.ToString() + " °C";

	// Update the associated image
	SetTemperatureImageFromMeasure(temperatureValue);
}

void IoTLab_Temperatures::MainPage::UpdateUserCoordinatesFromFields()
{
	// Retrieve the inputed coordinates
	Platform::String^ latitudeValue = LatitudeBox->Text;
	Platform::String^ latitudeSign = LatitudeSignComboBox->SelectedItem->ToString();
	Platform::String^ formattedLatitude = latitudeSign + latitudeValue;

	Platform::String^ longitudeValue = LongitudeBox->Text;
	Platform::String^ longitudeSign = LongitudeSignComboBox->SelectedItem->ToString();
	Platform::String^ formattedLongitude = longitudeSign + longitudeValue;

	// Compute the user's coordinates and retrieve the closest mote's measures
	userCoordinate = GeographicCoordinate(typeConversion::ToDouble(formattedLatitude), typeConversion::ToDouble(formattedLongitude));
}

// Enable the "Validate" button depending of the validity of the other fields
void IoTLab_Temperatures::MainPage::UpdateValidateButtonValidity()
{
	ValidateButton->IsEnabled = IsLatitudeValid()
		&& IsLongitudeValid();
}

void IoTLab_Temperatures::MainPage::UpdateLocationData(Windows::Devices::Geolocation::Geoposition^ position)
{
	position != nullptr
		? SetGeolocationPropertiesText(
			position->Coordinate->Point->Position.Latitude.ToString(),
			position->Coordinate->Point->Position.Longitude.ToString())
		: SetGeolocationPropertiesText("No data", "No data");
}

// On click, build the user's geographic coordinate from the latitude and the longitude input fields
// and display it
void IoTLab_Temperatures::MainPage::ValidateButton_Click(
	Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	UpdateUserCoordinatesFromFields();

	// Fire the event requesting for the app to asynchronously compute the closest
	// mote of the user, according to his updated coordinates
	SetEvent(hUpdateClosestMoteEvent);

	// Fire the event requesting for the app to asynchronously retrieve the latest
	// measure report of the closest mote
	SetEvent(hUpdateMoteMeasureReportEvent);
}

void IoTLab_Temperatures::MainPage::CompassImage_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	// TODO: Open the Windows map and give an itinerary to the user
	DirectionValueTextBlock->Text = "Open Map";
}
