/*
   Code for getting calibration values from a Soil Moisture Sensor

   Author: Tristan Fivaz

   Occupation: Townsville City Council Sustainability Support Officer

   Version 1.0

   Recent Update: None
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.print("Raw Analog Reading: ");
  Serial.println(sensorValue);
  delay(100);        // delay in between reads for stability
}
