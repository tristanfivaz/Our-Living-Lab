/*
   Code for sending Temperature, Humidity and Lux Readings from Sensors to ThingSpeak

   Author: Tristan Fivaz

   Occupation: Townsville City Council Sustainability Support Officer

   Version 1.0

   Recent Update: None
*/

/* Import Libraries  */
#include <ESP8266WiFi.h>
#include "DHT.h"

/* Definitions */
#define DHTPIN D3
#define DHTTYPE DHT11

const int analog_pin = A0;      // Analog light sensor pin

// Calibration values (adjust these based on your sensor and environment)
const float voltage_min = 0.0;  // Minimum sensor voltage (corresponding to 0 lux)
const float voltage_max = 3.3;  // Maximum sensor voltage (corresponding to maximum lux)

const int lux_min = 0;          // Minimum lux value
const int lux_max = 10000;      // Maximum lux value

float voltage_to_lux(float voltage_value) {
  // Convert sensor voltage to lux using calibration constants
  float slope = (lux_max - lux_min) / (voltage_max - voltage_min);
  float intercept = lux_min - slope * voltage_min;
  float lux = slope * voltage_value + intercept;
  return lux;
}

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  int analog_value = analogRead(analog_pin);
  float sensor_voltage = analog_value * (3.3 / 1023.0);
  float temperature = dht.readTemperature();    // Reads temperature as Celsius (the default)
  float humidity = dht.readHumidity();          // Reads humidity %

  // Convert sensor voltage to lux
  float lux_value = voltage_to_lux(sensor_voltage);

  // Print the result
  Serial.print("Light Sensor voltage: ");
  Serial.println(sensor_voltage, 2);
  Serial.print("Lux value: ");
  Serial.println(lux_value, 2);
  Serial.print("Temperature: ");
  Serial.println(temperature, 2);
  Serial.print("Humidity: ");
  Serial.println(humidity, 2);
  Serial.println(" ");

  delay(2000);
}
