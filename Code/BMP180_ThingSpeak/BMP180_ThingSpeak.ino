/*
   Code for sending Temperature, Humidity and Lux Readings from Sensors to ThingSpeak

   Author: Tristan Fivaz

   Occupation: Townsville City Council Sustainability Support Officer

   Version 1.0

   Recent Update: None
*/

/* Import Libraries  */
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

/* Definitions */
#define seaLevelPressure_hPa 1013.25

const int sleepTimeS = 900;

/* Wi-Fi Credentials */
char ssid[] = "YOUR-NETWORK-SSID";                            // Your network SSID (name)
char pswd[] = "YOUR-NETWORK-PASSWORD";                        // Your network password
int status = WL_IDLE_STATUS;
WiFiClient  client;

/* ThingSpeak Credentials  */
unsigned long myChannelNumber = 00000000000000;               // Enter your channel number from ThingSpeak
const char * myWriteAPIKey = "YOUR-WRITE-API-KEY";            // Enter your Write API Key from ThingSpeak

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);
  ThingSpeak.begin(client);
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  Serial.print("I'm Awake");
  delay(2000);

  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }

  float temperature = bmp.readTemperature();
  float altitude = bmp.readAltitude();
  float realAltitude = bmp.readAltitude(seaLevelPressure_hPa * 100);
  float pressure = bmp.readPressure();
  float seaLevelPressure = bmp.readSealevelPressure();

  // Print the result
  Serial.print("Temperature (C): ");
  Serial.println(temperature, 2);
  Serial.print("Altitude (m): ");
  Serial.println(altitude, 2);
  Serial.print("Real Altitude (m): ");
  Serial.println(realAltitude, 2);
  Serial.print("Pressure in Deployment Area (PA): ");
  Serial.println(pressure, 2);
  Serial.print("Pressure at Sea Level (PA): ");
  Serial.println(seaLevelPressure, 2);
  Serial.println(" ");

  //ThingSpeak.setField(Field No. , data)
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, realAltitude);
  ThingSpeak.setField(3, pressure);
  ThingSpeak.setField(4, seaLevelPressure);

  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  delay(2000);
  Serial.println("Back to sleep");
  ESP.deepSleep(sleepTimeS * 1000000);
}

void loop() {
}

void connectWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pswd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address :");
  Serial.println(WiFi.localIP());
  Serial.println();
}
