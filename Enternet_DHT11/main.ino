#include <SPI.h>                 // Библиотека SPI шины
#include <Ethernet.h>            // Ethernet библиотека
#include <PubSubClient.h>        // Библиотека MQTT
#include <DHT.h> 

//#define DHTPIN 3
//#define DHTPIN 5                 // Номер пина, к которому подсоединен датчик
#define DHTTYPE DHT11            // Задаём тип DHT датчика
//DHT dht1(DHTPIN, DHTTYPE);
//DHT dht2(DHTPIN, DHTTYPE);

DHT dht1(3, DHT11);
DHT dht2(5, DHT11);

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
{
  // 1 бод равно 0.8 бит/сек
  // 1 бит/сек равно 1.25 бод
  Serial.begin(9600);             // Задаём скорость порта в БОД'ах.
  Serial.println("DHT11 test!");  // Тестовое сообщ. при откр. Монитора порта

  dht1.begin();
  dht2.begin();
  
  Ethernet.begin(mac, ip);        // Инициализируем mac, ip
}

void loop()
{
  
  int h = dht1.readHumidity();     // Переменная типа int для Влажности
  int t = dht1.readTemperature();  // Переменная типа int для Температуры
  int h1 = dht2.readHumidity();     // Переменная типа int для Влажности
  int t1 = dht2.readTemperature();  // Переменная типа int для Температуры

  // Преобразуем переменные для отправки в MQTT в Брокер
  static char char_temp[10];      // Переменная для перевода из int в char
  dtostrf(t, 3, 0, char_temp);    // Перевод из int в char

  static char char_hum[10];
  dtostrf(h, 3, 0, char_hum);
  
  // Преобразуем переменные для отправки в MQTT в Брокер
  static char char_temp1[10];      // Переменная для перевода из int в char
  dtostrf(t1, 3, 0, char_temp1);    // Перевод из int в char

  static char char_hum1[10];
  dtostrf(h1, 3, 0, char_hum1);

  if (isnan(t) || isnan(h))
  if (isnan(t1) || isnan(h1))     // Проверка удачно ли прошло считывание с DHT22
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
  {
    Serial.print("Humidity: ");
    Serial.print(h1);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t1);
    Serial.println(" *C"); 
  }
  if (client.connect("DHTClient"))
  {
    //Отправка данных по MQTT в Брокер
    client.publish("home/data/status/sensor/temp", char_temp); //отправляем в Брокер значения
    client.publish("home/data/status/sensor/hum", char_hum);
    client.publish("home/data/status/sensor1/temp", char_temp1); //отправляем в Брокер значения
    client.publish("home/data/status/sensor1/hum", char_hum1);
    client.subscribe("inhome/data/status/sensor/#");       // (тестовая) для отпр. данных в MajorDoMo вручную
    delay(3000);              // Отправка данных в Брокер раз в 5 секунд

    client.disconnect();      // Отключиться
  }
}