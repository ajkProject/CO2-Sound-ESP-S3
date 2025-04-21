class DataRecord
{

    public:

        DataRecord();
        void UpdateDataRecord( int co2, int sound, int maxIntSound, int hour, int minute );        
        

        
        int GetCO2(); 
        int GetSound();
        int GetIntMaxSound(); //Max sound for this interval
        int GetHour(); 
        int GetMinute(); 

        void SetCO2(int co2);
        void SetSound(int sound); 
        void SetIntMaxSound(int maxIntSound); //Max sound for this interval 
        void SetTime(int hour, int minute);

        private:
        int co2;
        int sound;
        int maxIntSound; //Max sound for this interval
        int time;
};