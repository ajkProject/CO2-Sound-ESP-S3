#include "extendedDataRecord.h"


ExtendedDataRecord::ExtendedDataRecord():DataRecord(), temp(0), humidity(0), minIntSound(0) // Initialize member variables

{
}

void ExtendedDataRecord::SetTemp(float temp) { this->temp = temp; }
void ExtendedDataRecord::SetHumidity(float humidity) { this->humidity = humidity; } 
void ExtendedDataRecord::SetIntMinSound(int minIntSound) { this->minIntSound = minIntSound; } //Max sound for this interval

float ExtendedDataRecord::GetTemp() { return temp; }
float ExtendedDataRecord::GetHumidity() { return humidity; }
int ExtendedDataRecord::GetIntMinSound() { return minIntSound; } //Max sound for this interval
