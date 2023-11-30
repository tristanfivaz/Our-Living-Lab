/*
   Code for sending expected Carbon Dioxide and TVOC values to ThingSpeak

   Author: Tristan Fivaz

   Occupation: Townsville City Council Sustainability Support Officer

   Version 1.0

   Recent Update: None
*/

/* Import Libraries  */
#include <Wire.h>
#include "Adafruit_SGP30.h"
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

Adafruit_SGP30 sgp;

/* Wi-Fi Credentials */
char ssid[] = "YOUR-NETWORK-SSID";                      // Your network SSID (name)
char pswd[] = "YOUR-NETWORK-PASSWORD";                  // Your network password
int status = WL_IDLE_STATUS;
WiFiClient client;

/* ThingSpeak Credentials */
unsigned long myChannelNumber = 0000000;                // Enter your channel number from ThingSpeak
const char *myWriteAPIKey = "YOUR-WRITE-API-KEY";       // Enter your Write API Key from ThingSpeak

/* return absolute humidity [mg/m^3] with approximation formula
* @param temperature [°C]
* @param humidity [%RH]
*/
const int waitTime = 900000;                             // The time between uploads in milliseconds
uint32_t getAbsoluteHumidity(float temperature, float humidity)
{
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);                                                                     // [mg/m^3]
  return absoluteHumidityScaled;
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(10);
  } // Wait for serial console to open!

  Serial.println("SGP30 test");

  if (!sgp.begin())
  {
    Serial.println("Sensor not found :(");
    while (1);
  }

  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);

  sgp.setIAQBaseline(0x98C4, 0x9E7B);                     // Set your baseline here

  // Connect to Wi-Fi
  connectWiFi();
  ThingSpeak.begin(client);
}

void loop()
{
  // If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humidity compensation for the air quality signals
  //float temperature = 22.1; // [°C]
  //float humidity = 45.2; // [%RH]
  //sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));

  if (!sgp.IAQmeasure())
  {
    Serial.println("Measurement failed");
    return;
  }
  Serial.print("TVOC ");
  Serial.print(sgp.TVOC);
  Serial.print(" ppb\t");
  Serial.print("eCO2 ");
  Serial.print(sgp.eCO2);
  Serial.println(" ppm");

  if (!sgp.IAQmeasureRaw())
  {
    Serial.println("Raw Measurement failed");
    return;
  }
  Serial.print("Raw H2 ");
  Serial.print(sgp.rawH2);
  Serial.print(" \t");
  Serial.print("Raw Ethanol ");
  Serial.print(sgp.rawEthanol);
  Serial.println("");

  //ThingSpeak.setField(Field No., data)
  ThingSpeak.setField(1, sgp.eCO2);
  ThingSpeak.setField(2, sgp.TVOC);

  // Write to ThingSpeak
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  delay(waitTime);
}

void connectWiFi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pswd);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address:");
  Serial.println(WiFi.localIP());
  Serial.println();
}
