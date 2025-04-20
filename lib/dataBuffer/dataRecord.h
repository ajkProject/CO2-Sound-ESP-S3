class DataRecord
{

    public:

        DataRecord();
        void UpdateDataRecord( int co2, int sound, int hour, int minute );        
        

        
        int GetCO2(); 
        int GetSound();
        int GetHour(); 
        int GetMinute(); 

        void SetCO2(int co2);
        void SetSound(int sound);  
        void SetTime(int hour, int minute);

        private:
        int co2;
        int sound;
        int time;
};