
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>

#define SEALEVELPRESSURE_HPA (1013.25) // Used for estimating altitude

const char *ssid = "TeamGreen-RPi";        // SSID broadcasted by Raspberry Pi
const char *wifi_password = "TeamGreen13"; // WPA2 Passphrase for authentication

const char *mqtt_server = "134.114.106.62";   // MQTT Broker Local IP address
const char *mqtt_topic = "home/measurements"; // MQTT Topic for broker
const char *clientID = "6";                   // Current Client ID

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

BH1750 lightMeter;
Adafruit_BME280 bme;

String temperature, pressure, alt, humidity, lux;

void setup()
{

  Serial.begin(115200);

  Serial.print("Connecting to ");  // Print that we are connecting
  Serial.println(ssid);            // Print what SSID we're connecting to
  WiFi.begin(ssid, wifi_password); // Connect to SSID using WPA2 Passphrase

  while (WiFi.status() != WL_CONNECTED) // While not connected
  {
    delay(500);        // Wait to connect
    Serial.print("."); // Print dot for feedback
  }
  Serial.println("WiFi connected"); // Print once connected
  if (client.connect(clientID))     // Try and connect to Raspberry Pi Broker
  {
    Serial.println("Connected to MQTT Broker!"); // Print success statement to serial
  }
  else
  {
    Serial.println("Connection to MQTT Broker failed..."); // Print failed statement to serial
  }

  Wire.begin();       // Set up Wire for the BH1750
  lightMeter.begin(); // Begin the light meter listening on i2c

  if (!bme.begin(0x76)) // Try to start BME sensor
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!"); // If BME not found print
    while (1)
      ; // Infinite loop to freeze code execution
  }
  delay(1000); // Let everything boot up
}

void loop()
{
  getValues();                                                                                                     // Call get values for sensor readings
  String payload = (String)clientID + "," + temperature + "," + pressure + "," + alt + "," + humidity + "," + lux; // Put everything into a single string

  if (client.publish(mqtt_topic, String(payload).c_str())) // If connected to MQTT server, push the payload string
  {
    Serial.println("Payload Sent."); // Print that payload sent
  }
  else
  {
    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again"); // Else not connected to broker, print so
    client.connect(clientID);                                                               // Try to connect again
    delay(100);                                                                             // Delay inbetween connect and publish calls
    client.publish(mqtt_topic, String(payload).c_str());                                    // Try to publish string again
  }
  delay(2000);          // Let everything finish up in preparation for deep sleep
  ESP.deepSleep(900e6); // Deep Sleep for 15 minutes
}

void getValues()
{
  Serial.println("");                                   // Print blank space before debug sensor print
  float fTemp = bme.readTemperature() * 9.0 / 5.0 + 32; // Read BME temp and convert to fahrenheit
  Serial.print(fTemp);                                  // Print temperature
  Serial.println(" *F");                                // Print units

  float pres = bme.readPressure() / 100.0F; // Read pressure and convert to hPa
  Serial.print(pres);                       // Print pressure
  Serial.println(" hPa");                   // Print units

  float alti = bme.readAltitude(SEALEVELPRESSURE_HPA) * 3.2808; // Estimate altitude using pressure and convert to feet
  Serial.print(alt);                                            // Print altitude
  Serial.println(" ft");                                        // Print units

  float rH = bme.readHumidity(); // Read humidity from BME
  Serial.print(rH);              // Print humidity
  Serial.println(" %");          // Print units

  float light = lightMeter.readLightLevel(); // Read light levels
  Serial.print(lux);                         // Print light level
  Serial.println(" lx");                     // Print units

  temperature = (String)fTemp; // Convert temperature to string
  pressure = (String)pres;     // Convert pressure to string
  alt = (String)alti;          // Convert altitude to string
  humidity = (String)rH;       // Convert humidity to string
  lux = (String)light;         // Convert light to string
}
