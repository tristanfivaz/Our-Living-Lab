/*
   Code for getting a lux reading from a Light Sensor

   Author: Tristan Fivaz

   Occupation: Townsville City Council Sustainability Support Officer

   Version 1.0

   Recent Update: None
*/

// Analog light sensor pin
const int analog_pin = A0;

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

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Read analog value from the sensor and convert it to voltage
  int analog_value = analogRead(analog_pin);
  float sensor_voltage = analog_value * (3.3 / 1023.0);

  // Convert sensor voltage to lux
  float lux_value = voltage_to_lux(sensor_voltage);

  // Print the result
  Serial.print("Analog value: ");
  Serial.println(analog_value);
  Serial.print("Sensor voltage: ");
  Serial.println(sensor_voltage, 2);  // Display voltage with 2 decimal places
  Serial.print("Lux value: ");
  Serial.println(lux_value, 2);  // Display lux with 2 decimal places
  Serial.println(" ");

  delay(1000);  // Wait for 1 second
}
