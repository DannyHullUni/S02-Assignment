// Developer note stack size needs to be increased to allow the allocation of memory for this double = 8 bytes
// 8 * 24 * 60 * 60 * 2 arrays of doubles = 1382400 bytes base allocation is 1MB 
// In VS2017 go to project > properties > System > and set Stack Reserve Size and Stack Commit Size to 2000000
// This prevents a stack overflow

#define maximum_readings 5
#define invalid_reading  -1000.0


#define fakedata 1	 // NOTE:  Set to 1 use fake sensor values instead of the online values 0

#include <chrono>  // System time
#include <stdexcept>

#if fakedata == 1

#include "fakesensor.hpp"

#else

#include "http.h"

#endif


using namespace std;
using namespace chrono;


class Climate {

private:

#if fakedata == 1	
	FakeSensor sensorDevice;
#else
	Http sensorDevice;
#endif

protected:

public:

	// Constructors
	Climate();

	// Utility
	void	clearSamples();

	// Sensor related
	long	readSensor();
	long	sampleCount(long);
	long	sampleTotal(); 

	//system start time 
	system_clock::time_point startTime;

	float tempArray[maximum_readings];
	float humidArray[maximum_readings];
};

// Constructor
Climate::Climate() {
	startTime = std::chrono::system_clock::now();
}


void	Climate::clearSamples() {
	
}


long	Climate::readSensor() {

		system_clock::time_point timeNow = chrono::system_clock::now();

		std::chrono::duration<double> duration = timeNow - startTime;
 
		int currentSecond = (int)duration.count();

		sensorDevice.read_data();

		tempArray[currentSecond] = sensorDevice.get_temperature_in_c();
		humidArray[currentSecond] = sensorDevice.get_humidity();

		// This line is purely for your debugging and can be removes/commented out in the final code.
		cout << endl << "Debugging information : " << "Temperature is " << sensorDevice.get_temperature_in_c() << " in degrees C " << sensorDevice.get_humidity() << "% humidity" << endl;

		cout << endl << "Tempreature stored in Array:  " << tempArray[currentSecond] << " C" << endl;
		cout << "Humidity stored in Array:  " << humidArray[currentSecond] << "%" << endl;

		return currentSecond;

}

long Climate::sampleCount(long samples)
{
	//number of samples taken in time period NOT total number of samples 



	return (long) samples;
}

long Climate::sampleTotal()
{
	
}