// Developer note stack size needs to be increased to allow the allocation of memory for this double = 8 bytes
// 8 * 24 * 60 * 60 * 2 arrays of doubles = 1382400 bytes base allocation is 1MB 
// In VS2017 go to project > properties > System > and set Stack Reserve Size and Stack Commit Size to 2000000
// This prevents a stack overflow



#define maximum_readings 24 * 60 * 60 
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
	long	sampleCount(long numOfSec);
	long	sampleTotal();

	// Sensor related
	long	readSensor();
	double	getHumidity(long numOfSec);
	double	getTemperature(long numOfSec);

	//Humidity Methods
	double	minimumHumidity(long min);
	double	maximumHumidity(long max);
	double	averageHumidity(long avg);

	//Tempreature Methds
	double	minimumTemperature(long min);
	double	maximumTemperature(long max);
	double	averageTemperature(long avg);

	//system start time 
	system_clock::time_point startTime;
	//Arrays that store readings equal to how many the user wishes to see 
	float tempArray[maximum_readings]; //stores tempreature
	float humidArray[maximum_readings]; //store humidity 
};

// Constructor
Climate::Climate() {
	startTime = std::chrono::system_clock::now();
}

//Sets all samples, valid or invalid in each array to NULL, no value. 
void	Climate::clearSamples() {
		
	for (int index = 0; index <= maximum_readings; index++)
	{
		tempArray[index] = NULL;
		humidArray[index] = NULL; 
	}
}

long	Climate::readSensor() {

		system_clock::time_point timeNow = chrono::system_clock::now();
		
		sensorDevice.read_data();

		std::chrono::duration<double> duration = timeNow - startTime;
 
		int currentSecond = (int)duration.count();

		tempArray[currentSecond] = sensorDevice.get_temperature_in_c();
		humidArray[currentSecond] = sensorDevice.get_humidity();

		// This line is purely for your debugging and can be removes/commented out in the final code.
		//cout << endl << "Debugging information : " << "Temperature is " << sensorDevice.get_temperature_in_c() << " in degrees C " << sensorDevice.get_humidity() << "% humidity" << endl;

		//TEMPORARY CODE BELOW, REMOVE WHEN IMPLEMENTING GETTEMP AND GETHUMID METHODS

		//cout << endl << "Tempreature stored in Array:  " << tempArray[currentSecond] << " C" << endl;
		//cout << "Humidity stored in Array:  " << humidArray[currentSecond] << "%" << endl;

		return currentSecond;

}
//counts how many valid samples there are in both arrays in the current time frame
long Climate::sampleCount(long numOfSec)
{ 
	int		samples;

	if (numOfSec > maximum_readings || numOfSec < 1) { throw out_of_range ("Out of Range!"); }
	
	for (int index = numOfSec; index >= 1; index--) 
		{
			if (tempArray[index] != NULL)  {samples++;}
		}
		return samples;
}

long Climate::sampleTotal() 
{
	int		totalSamples;

	for (int index = 0; index < maximum_readings; index++) 
	{
		if (tempArray[index] != NULL) { totalSamples++; }
	}
	return totalSamples;
}


double Climate::getHumidity(long numOfSec)
{
	if (numOfSec > maximum_readings || numOfSec < 1) { throw out_of_range("Out of Range!"); }

	float humidity = humidArray[numOfSec];

	return humidity;
}

double Climate::getTemperature(long numOfSec)
{
	if (numOfSec > maximum_readings || numOfSec < 1) { throw out_of_range("Out of Range!"); }

	float temperature = tempArray[numOfSec];

	return temperature;
}

double Climate::minimumHumidity(long min) {

	double minimum = humidArray[min]; //assign minimum the humidity value of element 3600

	for (int index = min; index >= 1; index--) //i = 3600, run while i is greater than or equal to 1, i now equals 3599  
	{
		if (humidArray[index] < minimum) { minimum = humidArray[index]; } 
		// if humidArray[3600] value is less than minimum value 
		//minimum is now the humid[i]		
	}
	return minimum;
}

double Climate::minimumTemperature(long min) {

	double minimum = tempArray[min];

	for (int index = min; index >= 1; index--) 
	{ 
		if (tempArray[index] < minimum) { minimum = tempArray[index]; }
	}
	return minimum;
}

double Climate::maximumHumidity(long max)
{
	double maximum = humidArray[max];

	for (int index = max; index >= 1; index--)
	{
		if (humidArray[index] > maximum) { maximum = humidArray[index]; }
	}

	return maximum;
}

double Climate::maximumTemperature(long max)
{
	double maximum = tempArray[max];

	for (int index = max; index >= 1; index--)
	{
		if (tempArray[index] > maximum) { maximum = tempArray[index]; }
	}

	return maximum;
}

double Climate::averageHumidity(long avg)
{
	double average = 0.0; 
	double total = 0.0; 

	for (int index = avg; index >= 1; index--) 
	{
		total = total + humidArray[index];
	}
	
	average = total / avg;
	
	return average;
}

double Climate::averageTemperature(long avg)
{
	double average = 0.0;
	double total = 0.0;

	for (int index = avg; index >= 1; index--)
	{
		total = total + tempArray[index];
	}

	average = total / avg;

	return average;
}