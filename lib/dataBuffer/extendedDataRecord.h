#ifndef EXTENDEDDATARECORD_H
#define EXTENDEDDATARECORD_H

#include "dataRecord.h"

class ExtendedDataRecord: public DataRecord
{

    public:

        ExtendedDataRecord();       
        

        
        float GetTemp(); 
        float GetHumidity();
        int GetIntMinSound(); 

        void SetTemp(float temp);
        void SetHumidity(float humidity);
        void SetIntMinSound(int minIntSound); //Max sound for this interval 

        private:
        float temp;
        float humidity; 
        int minIntSound; //Min sound for this interval
       
};

#endif