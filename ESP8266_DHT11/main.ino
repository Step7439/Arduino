#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <OneWire.h>

#include "DHT.h"
#define DHTTYPE DHT11// DHT 11
//#define DHTPIN 5 // what digital pin we're connected to NodeMCU (D6)
//#define DHTPIN 2

DHT dht1(2, DHT11);
DHT dht2(5, DHT11);

// Uncomment whatever type you're using!


//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors. This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.


//DHT dht(DHTPIN, DHTTYPE);
//DHT dht1(DHTPIN, DHTTYPE);

#define wifi_ssid "House"
#define wifi_password "347620step"

#define mqtt_server "192.168.10.13"
//#define mqtt_user "user"
//#define mqtt_password "password"

#define humidity_topic "sensor/humidity"
#define temperature_celsius_topic "sensor/temperature_celsius"
#define humidity_topic1 "sensor/humidity1"
#define temperature_celsius_topic1 "sensor/temperature_celsius1"
//#define temperature_fahrenheit_topic "sensor/temperature_fahrenheit"

WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
    
          // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;  
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);
      clientName += "-";
      clientName += String(micros() & 0xff, 16);
      Serial.print("Connecting to ");
      Serial.print(mqtt_server);
      Serial.print(" as ");
      Serial.println(clientName);


    // Attempt to connect
    // 
    
  if (client.connect((char*) clientName.c_str())) {
 // if (client.connect((char*) clientName.c_str(),  mqtt_user, mqtt_password))) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
 }
}


void loop() {
  
      if (!client.connected()) {
        reconnect();
      }
      client.loop();
       byte i;
       byte present = 0;
       byte type_s;
       byte data[12];
       byte addr[8];
       
      // Wait a few seconds between measurements.
      delay(2000);
      
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      int h1 = dht1.readHumidity();
      // Read temperature as Celsius (the default)
      int t1 = dht1.readTemperature();

      int h2 = dht2.readHumidity();
      // Read temperature as Celsius (the default)
      int t2 = dht2.readTemperature();
     
      
      
      // Check if any reads failed and exit early (to try again).
     /* if (isnan(h1) || isnan(t1)) {
      Serial.println("Failed to read from DHT1 sensor!");
      return;
      }
      if (isnan(h2) || isnan(t2)) {
      Serial.println("Failed to read from DHT2 sensor!");
      return;
      }*/
      // Compute heat index in Fahrenheit (the default)
//      float hif = dht.computeHeatIndex(f, h);
      // Compute heat index in Celsius (isFahreheit = false)
      int hic1 = dht1.computeHeatIndex(t1, h1, false);
      int hic2 = dht2.computeHeatIndex(t2, h2, false);
     
      Serial.print("Temperature in Celsius1:");
      Serial.println(String(t1).c_str());
      client.publish(temperature_celsius_topic, String(t1).c_str(), true);


      Serial.print("Humidity1:");
      Serial.println(String(h1).c_str());
      client.publish(humidity_topic, String(h1).c_str(), true);

      Serial.print("Temperature in Celsius2:");
      Serial.println(String(t2).c_str());
      client.publish(temperature_celsius_topic1, String(t2).c_str(), true);


      Serial.print("Humidity2:");
      Serial.println(String(h2).c_str());
      client.publish(humidity_topic1, String(h2).c_str(), true);

  delay(5000);

//End of the OneWire-Devices, reset Loop
Serial.println("End of Onewire Bus");
//ds.reset_search();
return;
}