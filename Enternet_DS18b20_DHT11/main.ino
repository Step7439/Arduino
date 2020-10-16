#include <SPI.h>                 // Библиотека SPI шины
#include <Ethernet.h>            // Ethernet библиотека
#include <PubSubClient.h>        // Библиотека MQTT
#include <DHT.h> 
#include <OneWire.h>
#include <DS18B20.h>

                 // Номер пина, к которому подсоединен датчик
#define DHTTYPE DHT11            // Задаём тип DHT датчика
#define ONE_WIRE_BUS 5

DHT dht(3, DHT11);
OneWire oneWire(ONE_WIRE_BUS); //DS18B20 on GPIO2
DS18B20 sensor(&oneWire);

uint32_t start, stop;

// Задаём mac и ip адреса в Локальной сети
byte mac[]    = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip{192, 168, 10, 15};      //ip Адрес Ethernet Shild'a Arduino
IPAddress server{192, 168, 10, 13};  //ip Адрес для MQTT Брокера

// Шапка Функции Callback (обратный вызов)
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;                                 //Инициализируем Ethernet клиент
PubSubClient client(server, 1883, callback, ethClient);   //Инициализируем MQTT клиент

// Функция Callback
void callback(char* topic, byte* payload, unsigned int length)
{
  // Выделяем необходимое кол-во памяти для копии payload
  byte* p = (byte*)malloc(length);
  // Копирование payload в новый буфер
  memcpy(p, payload, length);
  client.publish("home/data/status/sensor", p, length);
  // Освобождаем память
  free(p);
}

// Функция Callback
void callback1(char* topic, byte* payload, unsigned int length)
{
  // Выделяем необходимое кол-во памяти для копии payload
  byte* p = (byte*)malloc(length);
  // Копирование payload в новый буфер
  memcpy(p, payload, length);
  client.publish("home/data/status/sensor1", p, length);
  // Освобождаем память
  free(p);
}
 
 void setup()
 {                         //18b20
  Ethernet.begin(mac, ip);
  Serial.begin(9600);                        
  Serial.println(__FILE__);
  Serial.print("DS18B20 Library version: ");
  Serial.println(DS18B20_LIB_VERSION);
  dht.begin();
  if (sensor.begin() == false)
  {
    Serial.println("Error: Could not find sensor.");
    while (1);
  }
  sensor.setResolution(12);
  sensor.requestTemperatures();

  start = millis();
  while (!sensor.isConversionComplete());
  stop = millis();

  Serial.print(stop - start);
  Serial.print("\t");
  Serial.println(sensor.getTempC(), 2);
  Serial.println();
}
                        //DHT11

  // 1 бод равно 0.8 бит/сек
  // 1 бит/сек равно 1.25 бод
  //Serial.begin(9600);             // Задаём скорость порта в БОД'ах.
  //Serial.println("DHT11 test!");  // Тестовое сообщ. при откр. Монитора порта

  
  
//  Ethernet.begin(mac, ip);        // Инициализируем mac, ip


void loop()
                           //18b20

{
  for (int res = 1; res < 2; res++)
  {
    
    sensor.setResolution(res);

    start = millis();
    sensor.requestTemperatures();
    while (!sensor.isConversionComplete());
    float temperature = sensor.getTempC();
    stop = millis();
    
    Serial.print("Temperature: ");
    Serial.println(temperature, 1); // 1 decimal makes perfect sense
    
    
  Serial.println();
  delay(5000);
  
  }
// -- END OF FILE --
{
  float temp1 = sensor.getTempC();
  int h = dht.readHumidity();     // Переменная типа int для Влажности
  int t = dht.readTemperature();  // Переменная типа int для Температуры

  // Преобразуем переменные для отправки в MQTT в Брокер
  static char char_temp[10];      // Переменная для перевода из int в char
  dtostrf(t, 3, 0, char_temp);    // Перевод из int в char

  static char char_hum[10];
  dtostrf(h, 3, 0, char_hum);

  static char char_temp1[10];      // Переменная для перевода из int в char
  dtostrf(temp1, 3, 0, char_temp1);


  // Преобразуем переменные для отправки в MQTT в Брокер
  
  if (isnan(t) || isnan(h))   // Проверка удачно ли прошло считывание с DHT22
  
  {
    Serial.println("Failed to read from DHT11");  // Не удалось прочитать DHT22
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");

  }

  if (client.connect("DHTClient"))
  {
    //Отправка данных по MQTT в Брокер
    client.publish("home/data/status/sensor/temp", char_temp); //отправляем в Брокер значения
    client.publish("home/data/status/sensor/hum", char_hum);
    client.publish("home/data/status/sensor/temp1", char_temp1);
    client.subscribe("inhome/data/status/sensor/#");       // (тестовая) для отпр. данных в MajorDoMo вручную
    delay(5000);              // Отправка данных в Брокер раз в 5 секунд
  
    client.disconnect();      // Отключиться
}
}
}