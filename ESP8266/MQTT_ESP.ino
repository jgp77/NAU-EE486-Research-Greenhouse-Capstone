
#include <ESP8266WiFi.h>  // Enables the ESP8266 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>

#define SEALEVELPRESSURE_HPA (1013.25)

const char *ssid = "TeamGreen-RPi";
const char *wifi_password = "TeamGreen13";

const char *mqtt_server = "134.114.106.62";
const char *mqtt_topic = "home/measurements";
const char *clientID = "6";

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker

BH1750 lightMeter;
Adafruit_BME280 bme;

String temperature, pressure, alt, humidity, lux;

void setup()
{

  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  if (client.connect(clientID))
  {
    Serial.println("Connected to MQTT Broker!");
  }
  else
  {
    Serial.println("Connection to MQTT Broker failed...");
  }

  Wire.begin();

  lightMeter.begin();

  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  delay(1000); // Let everything boot up
}

void loop()
{
  getValues();
  String payload = (String)clientID + "," + temperature + "," + pressure + "," + alt + "," + humidity + "," + lux;

  if (client.publish(mqtt_topic, String(payload).c_str()))
  {
    Serial.println("Payload Sent.");
  }
  else
  {
    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID);
    delay(100); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(mqtt_topic, String(payload).c_str());
  }

 delay(2000);
  ESP.deepSleep(900e6);

}

void getValues()
{
  Serial.println("");
  float fTemp = bme.readTemperature() * 9.0 / 5.0 + 32;
  Serial.print(fTemp);
  Serial.println(" *F");

  float pres = bme.readPressure() / 100.0F;
  Serial.print(pres);
  Serial.println(" hPa ");

  float alti = bme.readAltitude(SEALEVELPRESSURE_HPA) * 3.2808;
  Serial.print(alt);
  Serial.println(" ft");

  float rH = bme.readHumidity();
  Serial.print(rH);
  Serial.println(" %");

  float light = lightMeter.readLightLevel();
  Serial.print(lux);
  Serial.println(" lx");

  temperature = (String)fTemp;
  pressure = (String)pres;
  alt = (String)alti;
  humidity = (String)rH;
  lux = (String)light;
}
