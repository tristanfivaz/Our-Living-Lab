/*
   Code for sending Soil Moisture Percentage to ThingSpeak

   Author: Tristan Fivaz

   Occupation: Townsville City Council Sustainability Support Officer

   Version 1.0

   Recent Update: None
*/

/* Import Libraries  */
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

/* Definitions */
const int sleepTimeS = 900;
const int DRY = 772;                    // Value for dry soil
const int WET = 410;                    // Value for wet soil

/* Wi-Fi Credentials */
char ssid[] = "YOUR-WIFI-NAME";         // Your network SSID (name)
char pswd[] = "YOUR-WIFI-PASSWORD";     // Your network password
int status = WL_IDLE_STATUS;
WiFiClient  client;

/* ThingSpeak Credentials  */
unsigned long myChannelNumber = 00000000000000;               // Enter your channel number from ThingSpeak
const char * myWriteAPIKey = "YOUR-WRITE-API-KEY";            //Enter your Write API Key from ThingSpeak

void setup() {
  Serial.begin(115200);
  ThingSpeak.begin(client);
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  Serial.print("I'm Awake");
  delay(2000);

  int sensorValue = analogRead(A0);
  // map the raw reading to a percentage using DRY and WET values:
  int moisturePercentage = map(sensorValue, DRY, WET, 0, 100);

  Serial.print("Raw Analog Reading: ");
  Serial.println(sensorValue);
  Serial.print("Moisture Percentage: ");
  Serial.print(moisturePercentage);
  Serial.println("%");
  
  //ThingSpeak.setField(Field No. , data)
  ThingSpeak.setField(1, moisturePercentage);

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
