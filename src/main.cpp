#include <Arduino.h>

#include "SD2405.h"
#include "ArduinoJson.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "epd2in9_V2.h"
#include "ProgressBar.h"
#include "imagedata.h"
#include <SensirionI2cScd4x.h>
#include <Wire.h>
//#include "dataBuffer.h"
#include "graph.h"
#include "extendedDataRecord.h"

#define COLORED     0
#define UNCOLORED   1

#define BLACK     0
#define DARKGRAY   0
#define LIGHTGRAY  0
#define WHITE     3

 unsigned char image[4736];
 ProgressBar paint(image, 128, 296, false);    // width should be the multiple of 8 
 Epd epd;
 int frameCount = 0;
 unsigned long time_start_ms;
 unsigned long time_now_s;
 char time_string[] = {'0', '0', ':', '0', '0', '\0'}; 
 bool overnightTimeUpdate = false;

 //Declarations for WiFi
 const char* ssid = "Alexzz";
 const char* password = "23atKnoydart";
 String serviceName = "https://www.timeapi.io/api/time/current/zone?timeZone=Europe%2FLondon";
 String writeServiceName = "https://www.ajk.me.uk/api/Environmental/CO2Sound";
 const char* apiKey = "e1566d1869bc44ad966d1869bce4adad";
 HTTPClient http;

 const char* timeapiRootCa = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB\n" \
    "iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n" \
    "cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n" \
    "BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw\n" \
    "MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV\n" \
    "BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU\n" \
    "aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy\n" \
    "dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK\n" \
    "AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B\n" \
    "3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY\n" \
    "tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/\n" \
    "Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2\n" \
    "VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT\n" \
    "79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6\n" \
    "c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT\n" \
    "Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l\n" \
    "c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee\n" \
    "UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE\n" \
    "Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd\n" \
    "BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G\n" \
    "A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF\n" \
    "Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO\n" \
    "VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3\n" \
    "ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs\n" \
    "8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR\n" \
    "iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze\n" \
    "Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ\n" \
    "XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/\n" \
    "qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB\n" \
    "VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB\n" \
    "L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG\n" \
    "jjxDah2nGN59PRbxYvnKkKj9\n" \
    "-----END CERTIFICATE-------\n";

  const char* ajkMeUkRootCa = \
    "-----BEGIN CERTIFICATE-----\n" \
    "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\n" \
    "-----END CERTIFICATE-----\n";
  

 //Declarations for RTC
SD2405 rtc;

//Declaration for CO2 Sensor
// macro definitions
// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

SensirionI2cScd4x sensor;

static char errorMessage[64];
static int16_t error;

//Declarations of Sound Level

#define PCBARTISTS_DBM       0x48
 
#define I2C_REG_VERSION      0x00
#define I2C_REG_ID3          0x01
#define I2C_REG_ID2          0x02
#define I2C_REG_ID1          0x03
#define I2C_REG_ID0          0x04
#define I2C_REG_SCRATCH      0x05
#define I2C_REG_CONTROL      0x06
#define I2C_REG_TAVG_HIGH    0x07
#define I2C_REG_TAVG_LOW     0x08
#define I2C_REG_RESET        0x09
#define I2C_REG_DECIBEL      0x0A
#define I2C_REG_MIN          0x0B
#define I2C_REG_MAX          0x0C
#define I2C_REG_THR_MIN      0x0D
#define I2C_REG_THR_MAX      0x0E
#define I2C_REG_HISTORY_0    0x14
#define I2C_REG_HISTORY_99   0x77
 

DataBuffer *resultsBuffer; //Array of DataRecords


void printDate(Stream &str);
void printTime(Stream &str);
int wifiScan();

const char* months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void PrintUint64(uint64_t& value) {
  Serial.print("0x");
  Serial.print((uint32_t)(value >> 32), HEX);
  Serial.print((uint32_t)(value & 0xFFFFFFFF), HEX);
}

void WiFiEvent(WiFiEvent_t event){
  Serial.printf("[WiFi-event] event: %d\n", event);

  switch (event) {
      case ARDUINO_EVENT_WIFI_READY: 
          Serial.println("WiFi interface ready");
          break;
      case ARDUINO_EVENT_WIFI_SCAN_DONE:
          Serial.println("Completed scan for access points");
          break;
      case ARDUINO_EVENT_WIFI_STA_START:
          Serial.println("WiFi client started");
          break;
      case ARDUINO_EVENT_WIFI_STA_STOP:
          Serial.println("WiFi clients stopped");
          break;
      case ARDUINO_EVENT_WIFI_STA_CONNECTED:
          Serial.println("Connected to access point");
          break;
      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
          Serial.println("Disconnected from WiFi access point");
          break;
      case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
          Serial.println("Authentication mode of access point has changed");
          break;
      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
          Serial.print("Obtained IP address: ");
          Serial.println(WiFi.localIP());
          break;
      case ARDUINO_EVENT_WIFI_STA_LOST_IP:
          Serial.println("Lost IP address and IP address is reset to 0");
          break;
      case ARDUINO_EVENT_WPS_ER_SUCCESS:
          Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
          break;
      case ARDUINO_EVENT_WPS_ER_FAILED:
          Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
          break;
      case ARDUINO_EVENT_WPS_ER_TIMEOUT:
          Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
          break;
      case ARDUINO_EVENT_WPS_ER_PIN:
          Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
          break;
      case ARDUINO_EVENT_WIFI_AP_START:
          Serial.println("WiFi access point started");
          break;
      case ARDUINO_EVENT_WIFI_AP_STOP:
          Serial.println("WiFi access point  stopped");
          break;
      case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
          Serial.println("Client connected");
          break;
      case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
          Serial.println("Client disconnected");
          break;
      case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
          Serial.println("Assigned IP address to client");
          break;
      case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
          Serial.println("Received probe request");
          break;
      case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
          Serial.println("AP IPv6 is preferred");
          break;
      case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
          Serial.println("STA IPv6 is preferred");
          break;
      case ARDUINO_EVENT_ETH_GOT_IP6:
          Serial.println("Ethernet IPv6 is preferred");
          break;
      case ARDUINO_EVENT_ETH_START:
          Serial.println("Ethernet started");
          break;
      case ARDUINO_EVENT_ETH_STOP:
          Serial.println("Ethernet stopped");
          break;
      case ARDUINO_EVENT_ETH_CONNECTED:
          Serial.println("Ethernet connected");
          break;
      case ARDUINO_EVENT_ETH_DISCONNECTED:
          Serial.println("Ethernet disconnected");
          break;
      case ARDUINO_EVENT_ETH_GOT_IP:
          Serial.println("Obtained IP address");
          break;
      default: break;
  }}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));
}

bool ConnectWiFi()
{

  
  wifiScan();
  //WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.onEvent(WiFiEvent);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
      Serial.print("WiFi lost connection. Reason: ");
      Serial.println(info.wifi_sta_disconnected.reason);
  }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting WIFI");
  int count = 0;
  while((WiFi.status() != WL_CONNECTED)&&( count < 60)) {
    delay(500);
    Serial.print(".");
    count ++;
  }

  if( WiFi.status() != WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("Failed to connect WiFi - "+ String(WiFi.status()));
    return false;
    
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  return true;

}

//Needs to return a list of wifi networks
int wifiScan()
{

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);


    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.printf("%2d",i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4ld", WiFi.RSSI(i));
            Serial.print(" | ");
            Serial.printf("%2ld", WiFi.channel(i));
            Serial.print(" | ");
            switch (WiFi.encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                Serial.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                Serial.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                Serial.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                Serial.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                Serial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                Serial.print("WAPI");
                break;
            default:
                Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

    return n;

}


bool SetRTCTime()
{

  WiFiClientSecure *client = new WiFiClientSecure;
  client->setCACert( timeapiRootCa ); 
  client->setTimeout(10000);

  // int connectVal = client.connect("www.timeapi.io", 443);
  // if (connectVal != 1) 
  // {
  //   Serial.println("Failed to connect to timeapi.io - error code: " + String(connectVal));
  // }

  http.begin(*client, serviceName);
  int httpResponseCode = http.GET();
  if(httpResponseCode>0)
  {
    String response = http.getString();  //Get the response to the request
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("Response: ");
    Serial.println(response);           //Print request answer
    JsonDocument doc;
    deserializeJson(doc, response);
    const char* dateTime = doc["dateTime"];
    Serial.print("Date/Time: ");
    Serial.println(dateTime);

    rtc.setSeconds(doc["seconds"].as<uint8_t>());
    rtc.setMinutes(doc["minute"].as<uint8_t>());
    rtc.setHours(doc["hour"].as<uint8_t>());
    rtc.setDay(doc["day"].as<uint8_t>());
    rtc.setMonth(doc["month"].as<uint8_t>());
    rtc.setYear(doc["year"].as<int>()-2000);

    rtc.enableWriteRTC();
    rtc.write();

  } 
  else 
  {
    Serial.print("Error on sending GET: ");
    Serial.println(http.errorToString(httpResponseCode));
    http.end(); //Free the resources
    client->stop(); //Close the connection
    delete client; //Delete the client object  
    return false;
  }

  http.end(); //Free the resources
  client->stop(); //Close the connection
  delete client; //Delete the client object
  return true;

}

bool SetUpCO2Sensor()
{


    sensor.begin(Wire, SCD41_I2C_ADDR_62);

    uint64_t serialNumber = 0;
    delay(30);
    // Ensure sensor is in clean state
    error = sensor.wakeUp();
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute wakeUp(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
    }
    error = sensor.stopPeriodicMeasurement();
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
    }
    error = sensor.reinit();
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute reinit(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
    }
    // Read out information about the sensor
    error = sensor.getSerialNumber(serialNumber);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute getSerialNumber(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return false;
    }
    Serial.print("serial number: ");
    PrintUint64(serialNumber);
    Serial.println();
    //
    // If temperature offset and/or sensor altitude compensation
    // is required, you should call the respective functions here.
    // Check out the header file for the function definitions.
    // Start periodic measurements (5sec interval)
    error = sensor.startPeriodicMeasurement();
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return false;
    }
    //
    // If low-power mode is required, switch to the low power
    // measurement function instead of the standard measurement
    // function above. Check out the header file for the definition.
    // For SCD41, you can also check out the single shot measurement example.
    //

    return true;
}

byte reg_read(byte addr, byte reg) 
{
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, (uint8_t)1);
  byte data = Wire.read();
  return data;
}

void reg_write(byte addr, byte reg, byte data) 
{
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

bool SetUpSoundLevel()
{

  reg_write(PCBARTISTS_DBM, I2C_REG_RESET, 0x02);
  byte version = reg_read(PCBARTISTS_DBM, I2C_REG_VERSION);
    Serial.print("dbMeter VERSION = 0x");
    Serial.println(version, HEX);
   
    byte id[4];
    id[0] = reg_read(PCBARTISTS_DBM, I2C_REG_ID3);
    id[1] = reg_read(PCBARTISTS_DBM, I2C_REG_ID2);
    id[2] = reg_read(PCBARTISTS_DBM, I2C_REG_ID1);
    id[3] = reg_read(PCBARTISTS_DBM, I2C_REG_ID0);
   
    Serial.print("Unique ID = ");
    Serial.print(id[0], HEX); 
    Serial.print(" ");
    Serial.print(id[1], HEX); 
    Serial.print(" ");
    Serial.print(id[2], HEX); 
    Serial.print(" ");
    Serial.println(id[3], HEX); 

    return true;
  
}

bool DisplayCO2(ExtendedDataRecord *currentRecord) {

    bool dataReady = false;
    uint16_t co2Concentration = 0;
    float temperature = 0.0;
    float relativeHumidity = 0.0;
    //
    // Slow down the sampling to 0.2Hz.
    //
    delay(5000);
    error = sensor.getDataReadyStatus(dataReady);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute getDataReadyStatus(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return false;
    }
    while (!dataReady) {
        delay(100);
        error = sensor.getDataReadyStatus(dataReady);
        if (error != NO_ERROR) {
            Serial.print("Error trying to execute getDataReadyStatus(): ");
            errorToString(error, errorMessage, sizeof errorMessage);
            Serial.println(errorMessage);
            return false;
        }
    }
    //
    // If ambient pressure compenstation during measurement
    // is required, you should call the respective functions here.
    // Check out the header file for the function definition.
    error =
        sensor.readMeasurement(co2Concentration, temperature, relativeHumidity);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return false;
    }
    //
    // Print results in physical units.
    Serial.print("CO2 concentration [ppm]: ");
    Serial.print(co2Concentration);
    Serial.println();
    Serial.print("Temperature [°C]: ");
    Serial.print(temperature);
    Serial.println();
    Serial.print("Relative Humidity [RH]: ");
    Serial.print(relativeHumidity);
    Serial.println();

    char currentCO2[16];
    sprintf(currentCO2, "%4dppm", co2Concentration);
    
    paint.DrawStringAt(9, 68, "CO", &Font16, DARKGRAY);
    paint.DrawStringAt(9 + Font16.Width*2, 74, "2", &Font12, DARKGRAY);
    paint.DrawStringAt(9 + Font16.Width*3, 68, currentCO2, &Font16, DARKGRAY);

    if( resultsBuffer->GetMaxCO2() != 0)
    {

        char minMaxCo2[30];

        sprintf(minMaxCo2, "Min %dppm  Max %dppm" , resultsBuffer->GetMinCO2(), resultsBuffer->GetMaxCO2() );

        paint.DrawStringAt(14, 86, minMaxCo2, &Font8, DARKGRAY);
        
    }

    currentRecord->SetCO2(co2Concentration);
    currentRecord->SetTemp(temperature);
    currentRecord->SetHumidity(relativeHumidity);

    return true;
}

void DisplayTime( ExtendedDataRecord *currentRecord)
 {

  char currentTime[16];
  sprintf(currentTime, "%02d:%02d", rtc.hours(), rtc.minutes());
  char currentDate[16];

  if(overnightTimeUpdate)
  {
    sprintf(currentDate, "%2d %s %04d.",rtc.day(), months[rtc.month()-1], 2000 + rtc.year());  
  }
  else
  {
    sprintf(currentDate, "%2d %s %04d",rtc.day(), months[rtc.month()-1], 2000 + rtc.year());
  }
  
  
  paint.DrawStringAt(4, 4, currentTime, &Font36, BLACK);  
  paint.DrawStringAt(9, 44, currentDate, &Font16, BLACK);

  currentRecord->SetTime(rtc.hours(), rtc.minutes());
  

}

void DisplaySoundLevel( ExtendedDataRecord *currentRecord)
{
  
  byte sound_level = reg_read(PCBARTISTS_DBM, I2C_REG_DECIBEL);
  Serial.print("Sound Level (dB SPL) = ");
  Serial.println(sound_level);

  char currentSoundLevel[16];
  sprintf(currentSoundLevel, "%3ddB", sound_level);
  
  byte min_sound_level = reg_read(PCBARTISTS_DBM, I2C_REG_MIN);
  Serial.print("Min Sound Level (dB SPL) = ");
  Serial.println(min_sound_level);

  char minSoundLevel[16];
  sprintf(minSoundLevel, "Min %3ddB", min_sound_level);
  
  byte max_sound_level = reg_read(PCBARTISTS_DBM, I2C_REG_MAX);
  Serial.print("Max Sound Level (dB SPL) = ");
  Serial.println(max_sound_level);

  //Clear min/max registers
  reg_write(PCBARTISTS_DBM, I2C_REG_RESET, 0x02);

  
  paint.DrawStringAt(9, 100, "Sound", &Font16, BLACK);
  paint.DrawStringAt(9 + Font16.Width * 6, 100, currentSoundLevel, &Font16, BLACK);
  
  if( resultsBuffer->GetMaxSound() != 0)
  {
    char soundLevel[30];
    sprintf(soundLevel, "Ave %ddB  Max %ddB", resultsBuffer->GetAverageSound(), resultsBuffer->GetMaxSound());
    paint.DrawStringAt(14, 114, soundLevel, &Font8, BLACK);
    
  }

  currentRecord->SetSound(sound_level);
  currentRecord->SetIntMaxSound(max_sound_level);
  currentRecord->SetIntMinSound(min_sound_level);
}

bool WriteDataToServer( ExtendedDataRecord *currentRecord)
{
  //Send the data to the server
  //https://www.ajk.me.uk/api/CO2Sound


  DynamicJsonDocument doc(2048);
  doc["APIKEY"] = apiKey;
  doc["STATIONID"] = 1;
  doc["CO2"] = currentRecord->GetCO2();
  doc["TEMPERATURE"] = currentRecord->GetTemp();
  doc["HUMIDITY"] = currentRecord->GetHumidity();
  doc["SOUND"] = currentRecord->GetSound(); 
  doc["INTERVALMINSOUND"] = currentRecord->GetIntMinSound();
  doc["INTERVALMAXSOUND"] = currentRecord->GetIntMaxSound();

     Serial.print("Max Sound Level ");
      Serial.println(currentRecord->GetIntMaxSound());


  // Serialize JSON document
  String json;
  serializeJson(doc, json);

  Serial.print("JSON: ");
  Serial.println(json);

  if( WiFi.status() != WL_CONNECTED)
  {
    ConnectWiFi();
  }


  WiFiClientSecure *client = new WiFiClientSecure;
  client->setCACert( ajkMeUkRootCa ); 
  client->setTimeout(20000);

  // int connectVal = client.connect("www.timeapi.io", 443);
  // if (connectVal != 1) 
  // {
  //   Serial.println("Failed to connect to timeapi.io - error code: " + String(connectVal));
  // }

  http.begin(*client, writeServiceName);
      
      // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");
            Serial.println("Header Content Type Set");
    http.addHeader("Content-Length", String( json.length() ));
            Serial.print("Header Content Lenght ");
            Serial.println(String( json.length() ));
    int httpResponseCode = http.POST(json);
      

    if(httpResponseCode>0)
    {
      String response = http.getString();  //Get the response to the request
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);           //Print request answer
      
    } 
    else 
    {
      Serial.print("Error on sending POST: ");
      Serial.println(http.errorToString(httpResponseCode));

      http.end();

      return false;

    }
      
    // Free resources
    http.end(); //Free the resources
    client->stop(); //Close the connection
    delete client; //Delete the client object

    return true;

}

void UpdateDisplay( )
{
  ExtendedDataRecord currentRecord;
  paint.SetRotate(ROTATE_90);
  
  paint.Clear(WHITE);
  paint.DrawRectangle(1, 1, 294, 126, DARKGRAY);

  DisplayTime( &currentRecord);
  DisplayCO2(&currentRecord);
  DisplaySoundLevel(&currentRecord);

  resultsBuffer->AddRecord(currentRecord);

  Serial.print("CO2 Max: ");
    Serial.println(resultsBuffer->GetMaxCO2());
    Serial.print("CO2 Min: ");
    Serial.println(resultsBuffer->GetMinCO2());
    Serial.print("Sound Max: ");
    Serial.println(resultsBuffer->GetMaxSound());
    Serial.print("Sound Min: ");
    Serial.println(resultsBuffer->GetMinSound());

  Graph *graph = new Graph(&paint, resultsBuffer, BLACK );

  char wifiStatus[2] = "$";

  if( WiFi.status() != WL_CONNECTED )
  {
    wifiStatus[0] = ' ';
  }
 
  paint.DrawStringAt(135, 107, wifiStatus, &Font16Symbol, BLACK);

  graph->DrawDoubleGraph( 148, 4, 144, 120, currentRecord.GetHour(), currentRecord.GetMinute());
  
  epd.SetFrameMemory_Partial(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
 
  epd.DisplayFrame_Partial();
  epd.WaitUntilIdle();

  //Update the weather server api with the current data

  WriteDataToServer(&currentRecord);
  Serial.println("Data sent to server");
  
}

void setup()
{
  Serial.begin(115200);


  Serial.println();

  resultsBuffer = new DataBuffer(60*24); // 1 days's records

  ConnectWiFi();

  http.setReuse(true);

  Wire.begin();
  if (rtc.begin() != SD2405_OK)
  {
    Serial.println("could not connect, check wires etc");
    // while (1);
  }

   Serial.println();

  Serial.println(__FILE__);
  Serial.print("SD2405_LIB_VERSION: ");
  Serial.println(SD2405_LIB_VERSION);
  Serial.println();

  //Get current date/time
  if( !SetRTCTime() )
  {
    Serial.println("Failed to set RTC time - using existing time");
  }

  SetUpCO2Sensor();
  SetUpSoundLevel();

  if (epd.Init() != 0) 
  //if (epd.Init_4Gray() != 0) 
  {
    Serial.print("e-Paper init failed");
    return;
  }

  epd.ClearFrameMemory(WHITE);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  
}


void loop()
{
  
  

  rtc.read();

  if( rtc.seconds() == 0 )
  {
  
     if(( rtc.hours() == 2 )&&( rtc.minutes() == 1 ))
     {

   
        if( WiFi.status() != WL_CONNECTED )
        {
            WiFi.reconnect();   
        }
        else
        {
          Serial.println("Already connected to WiFi");
        }
        if( WiFi.status() != WL_CONNECTED )
        {
          Serial.println("Time to update RTC time");
          overnightTimeUpdate = SetRTCTime();
        }
     }
   
    Serial.print(rtc.lastRead());
    Serial.print("\t");
    printDate(Serial);
    Serial.print(" ");
    printTime(Serial);
    Serial.println();
    UpdateDisplay();
  
  }

  delay(1000);
}

void printDate(Stream &str)
{
  char buffer[16];
  sprintf(buffer, "%04d-%02d-%02d",
          2000 + rtc.year(), rtc.month(), rtc.day());
  str.print(buffer);
}


void printTime(Stream &str)
{
  char buffer[16];
  sprintf(buffer, "%02d:%02d:%02d",
          rtc.hours(), rtc.minutes(), rtc.seconds());
  str.print(buffer);
}

