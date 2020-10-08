#include "DHT.h" 
#include <ESP8266WiFi.h> 
#include <PubSubClient.h> 
#include <ArduinoJson.h> 
#define DHTPIN 4 // Номер пина, который подключен к DHT22 
#define DHTTYPE DHT22 // Указываем, какой тип датчика мы используем 

byte mqtt_server[] = { 192, 168, 88, 254 }; // ip адресс 
mqtt сервера char buffer[10]; DHT dht(DHTPIN, DHTTYPE); // WiFi const 
char* WIFI_SSID = "your_SSID"; // Укажите ваш 
SSID const char* WIFI_PASS = "your_PASSWORD"; // Укажите ваш wifi пароль // Читаем MQTT сообщения 
void callback(char* topic, byte* payload, unsigned int length) { } WiFiClient wifiClient; 
PubSubClient client(mqtt_server, 1883, callback, wifiClient); 
void connect() { WiFi.persistent(false); 
WiFi.mode(WIFI_OFF); WiFi.mode(WIFI_STA); 
WiFi.begin(WIFI_SSID, WIFI_PASS); 

unsigned long wifiConnectStart = millis(); 
while (WiFi.status() != WL_CONNECTED) { if (WiFi.status() == WL_CONNECT_FAILED) { Serial.println("Ошибка соединения WIFI. Проверьте данные: "); 

Serial.println(); Serial.print("SSID: "); 
Serial.println(WIFI_SSID); 
Serial.print("Password: "); 
Serial.println(WIFI_PASS); 
Serial.println(); 
} delay(500); 
if(millis() - wifiConnectStart > 5000) { Serial.println("Ошибка соединения WiFi"); 
Serial.println("Попробуйте отправить обновленные параметры конфигурации."); 
return; } } 
Serial.println("WiFi соединение установлено"); 
Serial.println("IP адрес: "); 
Serial.println(WiFi.localIP()); 
Serial.println(); } 
void setup() { 
Serial.begin(9600); 
Serial.setTimeout(2000); // Ждем, пока все загрузится. 
while(!Serial) { } Serial.println("Устройство запущено"); 
Serial.println("-------------------------------------"); 
Serial.println("Запускаем DHT!"); Serial.println("-------------------------------------"); 
connect(); } // Собираем данные, складывем воедино и отправляем 
void report(double humidity, double tempC) { StaticJsonBuffer<500> jsonBuffer; 
JsonObject& root = jsonBuffer.createObject(); 
root["humidity"] = humidity; 
root["tempC"] = tempC; 
char payload[256]; 
root.printTo(payload, sizeof(payload)); 
char* s = payload; 
client.publish("ESP/Temp", s); 

Serial.println("Отправлено!"); } 
int timeSinceLastRead = 0; 
void loop() { bool toReconnect = false; 
if (WiFi.status() != WL_CONNECTED) { Serial.println("Disconnected from WiFi"); 
toReconnect = true; } 
if (toReconnect) { connect(); } 
if(timeSinceLastRead > 5000) { float h = dht.readHumidity(); 
float t = dht.readTemperature(); 
if (isnan(h) || isnan(t)) { Serial.println("Данных нет! Останавливаем цикл и запускаем по новой"); 
timeSinceLastRead = 0; 
return; } report(h, t); // Функция отправки данных timeSinceLastRead = 0; 
} delay(100); 
timeSinceLastRead += 100; 
client.loop(); }