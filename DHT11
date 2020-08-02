#ifdef TEMPVL_FEATURE 
#include "DHT.h" 

DHT dht1(34, DHT11); 
DHT dht2(35, DHT11); 
byte const MAX_TEMPVL_SENSORS = 4; 
byte const FULL_HEALTHVL = 100; 
byte const ZERO_HEALTHVL = 0; 
byte tempvlSensorHealth[MAX_TEMPVL_SENSORS]; 
byte tempvlModulHealth; 
byte numberOfDevices = 2; 
char objSensvl[MAX_TEMPVL_SENSORS][10] = {"dht1h", "dht1t", "dht2h", "dht2t"}; 
float current_tempvl[MAX_TEMPVL_SENSORS]; 
float tempvl_[MAX_TEMPVL_SENSORS]; 
void tempvlInit(){ 

pinMode(47, OUTPUT); 
pinMode(48, OUTPUT); 
dht2.begin(); 
dht1.begin(); 
timeStamp(); 

Serial.print("DTH-11 \n" ); 
Serial.print(objSensvl[MAX_TEMPVL_SENSORS][10]); 
tempvlModulHealth = FULL_HEALTHVL; 
timeStamp(); Serial.print(" Found "); 
Serial.print(numberOfDevices); 
Serial.print(" devices\n"); } 
void setModulTempvlHealth() { int sumvlSensorsHealth = 0; 
for (byte i = 0; 
i < MAX_TEMPVL_SENSORS; i++) { Serial.println(objSensvl[i]); 
sumvlSensorsHealth += tempvlSensorHealth[i]; } 
tempvlModulHealth = sumvlSensorsHealth / MAX_TEMPVL_SENSORS; } 
void tempvlWorks() { tempvl_[0] = dht1.readHumidity(); 

tempvl_[1] = dht1.readTemperature(); 
tempvl_[2] = dht2.readHumidity(); 
tempvl_[3] = dht2.readTemperature(); 

getTemperaturevl(0); 
getTemperaturevl(1); 
getTemperaturevl(2); 
getTemperaturevl(3); } 

void getTemperaturevl(byte b) { tempvlSensorHealth[b] = FULL_HEALTHVL; 
current_tempvl[b] = tempvl_[b]; 
if (!isnan(current_tempvl[b])) { timeStamp(); 

Serial.print(objSensvl[b]); 
Serial.print(": "); 
Serial.println(current_tempvl[b]); 

tempvl_[b] = (current_tempvl[b] - (char)current_tempvl[b]) * 100; 
if ((current_tempvl[b] > -1) && (current_tempvl[b] < 0)) { 
#ifdef MAJORDOMO_FEATURE sendRequestMinusM(objSensvl[b], "-", (int)current_tempvl[b], abs(tempvl_[b])); 
#endif } else { 
#ifdef MAJORDOMO_FEATURE sendRequestMinusM(objSensvl[b], "", (int)current_tempvl[b], abs(tempvl_[b])); 
#endif } } else { tempvlSensorHealth[b] = ZERO_HEALTHVL; 
timeStamp(); 

Serial.print(objSensvl[b]); 
Serial.print(": "); 
Serial.print("error\n"); } } 
#endif // TEMPVL_FEATURE
