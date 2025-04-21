
#include "dataRecord.h"


DataRecord::DataRecord()
    : co2(0), sound(0), time(0) // Initialize member variables
{
}


void DataRecord::UpdateDataRecord( int co2, int sound, int maxIntSound, int hour, int minute )
    {
        this->co2 = co2;
        this->sound = sound;
        this->maxIntSound = maxIntSound; //Max sound for this interval
        this->time = hour * 60 + minute;
    }

    
void DataRecord::SetCO2(int co2) { this->co2 = co2; }
void DataRecord::SetSound(int sound) { this->sound = sound; }   
void DataRecord::SetIntMaxSound(int maxIntSound) { this->maxIntSound = maxIntSound; } //Max sound for this interval
void DataRecord::SetTime(int hour, int minute) { this->time = hour * 60 + minute; }

int DataRecord::GetCO2()  { return co2; }
int DataRecord::GetSound() { return sound; }
int DataRecord::GetIntMaxSound() { return maxIntSound; } //Max sound for this interval
int DataRecord::GetHour()  { return time/60; }
int DataRecord::GetMinute()  { return time%60; }
