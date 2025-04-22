#include "dataBuffer.h"

DataBuffer::DataBuffer(int size) :saveIndex(0), size(size), maxCo2(0), minCo2(__INT_MAX__), maxSound(0), minSound(__INT_MAX__), firstGoodRecord(0), dataWrapped(false), averageSoundSum(0)
{
    // Initialize the buffer with empty DataRecord objects

    buffer = new DataRecord[size]; // Allocate memory for the buffer
}

DataBuffer::~DataBuffer()
{
    delete[] buffer; // Free the allocated memory
}

void DataBuffer::AddRecord(int co2, int sound, int maxIntSound, int hour, int minute)
{

    int deletingCO2 = buffer[saveIndex].GetCO2(); // Get the CO2 value of the record being replaced
    int deletingMaxSound = buffer[saveIndex].GetIntMaxSound(); // Get the sound value of the record being replaced
    int deletingMinSound = buffer[saveIndex].GetSound(); // Get the sound value of the record being replaced

       // Update min and max values
       if (co2 > maxCo2) maxCo2 = co2;
       if (co2 < minCo2) minCo2 = co2;
       if (maxIntSound > maxSound) maxSound = maxIntSound;
       if (sound < minSound) minSound = sound;

       if( dataWrapped == false) // If the buffer has not wrapped around
       {
           averageSoundSum += sound; // Add the sound level to the sum for average calculation
       }
       else
       {
           averageSoundSum -= deletingMinSound; // Subtract the sound level of the deleted record from the sum for average calculation
           averageSoundSum += sound; // Add the new sound level to the sum for average calculation
       }

   
    buffer[saveIndex].UpdateDataRecord(co2, sound, maxIntSound, hour, minute);

    if(deletingCO2 == maxCo2) // we are about to delete the maxCo2 value
    {
        // Find the new maxCo2 in the buffer

        maxCo2 = 0; // Reset maxCo2 to find the new maximum
        for (int i = 0; i < size; i++)
        {
            if (buffer[i].GetCO2() > maxCo2)
            {
                maxCo2 = buffer[i].GetCO2(); // Update maxCo2 to the new value
            }
        }
    }

    if(deletingCO2 == minCo2) // we are about to delete the minCo2 value
    {
        // Find the new minCo2 in the buffer

        minCo2 = __INT_MAX__; // Reset minCo2 to find the new maximum
        for (int i = 0; i < size; i++)
        {
            if (buffer[i].GetCO2() < minCo2)
            {
                minCo2 = buffer[i].GetCO2(); // Update maxCo2 to the new value
            }
        }
    }
    // Update min and max values
    if(deletingMaxSound == maxSound) // we are about to delete the maxSound value
    {
        // Find the new maxSound in the buffer

        maxSound = 0; // Reset maxSound to find the new maximum
        for (int i = 0; i < size; i++)
        {
            if (buffer[i].GetIntMaxSound() > maxSound)
            {
                maxSound = buffer[i].GetIntMaxSound(); // Update maxSound to the new value
            }
        }
    }
    if(deletingMinSound == minSound) // we are about to delete the minSound value
    {
        // Find the new minSound in the buffer

        minSound = __INT_MAX__; // Reset minSound to find the new maximum
        for (int i = 0; i < size; i++)
        {
            if (buffer[i].GetSound() < minSound)
            {
                minSound = buffer[i].GetSound(); // Update maxSound to the new value
            }
        }
    }   
    // Increment the save index and wrap around if necessary

    
    saveIndex = (saveIndex + 1) % size;

    if( saveIndex == 0 )
    {
        dataWrapped = true; // Set the flag to indicate that the buffer has wrapped around
    }

    if( dataWrapped == true )
    {
        firstGoodRecord = saveIndex; // Update the first good record index
    }
    
    
}

void DataBuffer::AddRecord(DataRecord record) // Add a record to the buffer
{
    AddRecord(record.GetCO2(), record.GetSound(), record.GetIntMaxSound(), record.GetHour(), record.GetMinute()); // Use the existing AddRecord method
}

int DataBuffer::GetMaxCO2() { return maxCo2; }
int DataBuffer::GetMinCO2() { return minCo2; }      
int DataBuffer::GetMaxSound() { return maxSound; }
int DataBuffer::GetMinSound() { return minSound; }
int DataBuffer::GetAverageSound() 
{  

    if( saveIndex == 0) // If the buffer is empty
        return 0; // Return 0 as the average sound level

    if( dataWrapped == false) // If the buffer has not wrapped around
        return averageSoundSum / saveIndex; // Return the average sound level

    return averageSoundSum / size; // If the buffer has wrapped around, return the average sound level

}

int DataBuffer::CreateIndexer() 
{ 
    return (firstGoodRecord+ 1) % size;
}

bool DataBuffer::GetNextRecord(int &co2, int &sound, int &maxIntSound, int &hour, int &minute, int &indexer) 
{ 
    if (indexer == saveIndex) // If we've looped through all records
        return false; // No more records to read

    co2 = buffer[indexer].GetCO2();
    sound = buffer[indexer].GetSound();
    maxIntSound = buffer[indexer].GetIntMaxSound(); 
    hour = buffer[indexer].GetHour();
    minute = buffer[indexer].GetMinute();

    indexer = (indexer + 1) % size; // Update the indexer for the next record

    return true; // Successfully retrieved a record
}

int DataBuffer::GetRecordCount() 
{ 
    if (dataWrapped == false) // If the buffer has not wrapped around
        return saveIndex; // Return the number of records in the buffer

    return size; // If the buffer has wrapped around, return the total size
}   
// The above code is a C++ implementation of a circular buffer that stores data records.
