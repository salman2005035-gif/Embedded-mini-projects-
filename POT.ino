#include <Arduino.h>
#include <math.h>

// ESP32 pin numbers
#define POT_PIN 34
#define LED_PIN 2

// Threshold
#define THRESHOLD 600

// Sea-level pressure in hPa
const float SEA_LEVEL_PRESSURE = 1013.25;

int potValue;
float pressure;
float altitude;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  analogReadResolution(12);
}

void loop() {
  // Read potentiometer
  potValue = analogRead(POT_PIN);

  // Simulate pressure between 950 and 1050 hPa
  pressure = 950.0 + (potValue / 4095.0) * 100.0;

  // Calculate estimated altitude in metres
  altitude = 44330.0 *
      (1.0 - pow(pressure / SEA_LEVEL_PRESSURE, 0.1903));

  // Print results
  Serial.print("Potentiometer: ");
  Serial.println(potValue);

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  // Blink LED if reading exceeds 600
  if (potValue > THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
    delay(250);

    digitalWrite(LED_PIN, LOW);
    delay(250);
  } else {
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }

  Serial.println("--------------------");
}