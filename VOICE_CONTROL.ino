#include <WiFi.h>
#include "AdafruitIO_WiFi.h"

// Wi-Fi
#define WIFI_SSID     "Xiaomi 14"
#define WIFI_PASSWORD "12121212"

// Adafruit IO
#define IO_USERNAME "Abdulla322132"
#define IO_KEY      "aio_qTMx74yt6YO6VoSsq3hEFBfUz9vj"

// Relay
#define RELAY_PIN 23

// Most relay modules are active LOW.
// Change these if your relay works in reverse.
#define RELAY_ON  LOW
#define RELAY_OFF HIGH

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY,
                   WIFI_SSID, WIFI_PASSWORD);

AdafruitIO_Feed *lightFeed = io.feed("light");

void handleLight(AdafruitIO_Data *data) {
  String command = data->toString();
  command.trim();
  command.toUpperCase();

  Serial.print("Received command: ");
  Serial.println(command);

  if (command == "ON" || command == "1") {
    digitalWrite(RELAY_PIN, RELAY_ON);
    Serial.println("LIGHT ON");
  }
  else if (command == "OFF" || command == "0") {
    digitalWrite(RELAY_PIN, RELAY_OFF);
    Serial.println("LIGHT OFF");
  }
  else {
    Serial.println("Unknown command");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  Serial.println("Connecting to Adafruit IO...");
  io.connect();

  // Listen for changes to the light feed
  lightFeed->onMessage(handleLight);
}

void loop() {
  io.run();
}