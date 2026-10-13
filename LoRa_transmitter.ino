#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Example pins for a separate SX1276/RFM95 module
#define LORA_SS   5
#define LORA_RST  14
#define LORA_DIO0 26

#define LORA_FREQ 865E6

void setup() {
  Serial.begin(115200);

  SPI.begin(18, 19, 23, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa initialization failed!");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("LoRa Transmitter Ready");
}

void loop() {
  LoRa.beginPacket();
  LoRa.print("Hello from ESP32!");
  LoRa.endPacket();

  Serial.println("Message sent!");

  delay(2000);
}