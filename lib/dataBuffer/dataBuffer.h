#include "dataRecord.h"

class DataBuffer
{
    public:
        DataBuffer(int size);

        ~DataBuffer();

        void AddRecord(int co2, int sound, int maxIntSound, int hour, int minute);
        void AddRecord(DataRecord record); // Add a record to the buffer

        int GetMaxCO2(); 
        int GetMinCO2();    
        int GetMaxSound(); 
        int GetMinSound(); 
        int GetAverageSound(); // Get the average sound level

        //Todo - do we pass out an index or just have one.
        int CreateIndexer(); 
        bool GetNextRecord(int &co2, int &sound, int &maxIntSound, int &hour, int &minute, int &indexer); // Get the next record and update the indexer

        int GetRecordCount();

    private:
        int saveIndex;
        int size;
        int maxCo2;
        int minCo2;
        int maxSound;
        int minSound;
        int averageSoundSum;
        int firstGoodRecord; // First record that is not empty
        bool dataWrapped;

        DataRecord *buffer; //Array of DataRecords

    //Need a wrapping buffer, how is time going to work?
    // Get record the GetNext?

};