#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

#define LED_PIN 2
#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
WebServer server(80);

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 IoT Control</title>

  <style>
    body {
      font-family: Arial;
      text-align: center;
      background: #f2f2f2;
      margin-top: 50px;
    }

    .box {
      background: white;
      width: 320px;
      margin: auto;
      padding: 25px;
      border-radius: 15px;
      box-shadow: 0 4px 10px rgba(0,0,0,0.2);
    }

    button {
      width: 120px;
      padding: 15px;
      margin: 10px;
      border: none;
      border-radius: 8px;
      font-size: 18px;
      cursor: pointer;
    }

    .on {
      background: #28a745;
      color: white;
    }

    .off {
      background: #dc3545;
      color: white;
    }

    .temperature {
      font-size: 35px;
      font-weight: bold;
      margin: 20px;
    }
  </style>
</head>

<body>

<div class="box">

  <h1>ESP32 IoT Control</h1>

  <h2>LED Control</h2>

  <button class="on" onclick="ledControl('ON')">
    ON
  </button>

  <button class="off" onclick="ledControl('OFF')">
    OFF
  </button>

  <p id="ledStatus">LED Status: Unknown</p>

  <hr>

  <h2>Temperature</h2>

  <div class="temperature">
    <span id="temperature">--</span> °C
  </div>

</div>

<script>

function ledControl(state) {

  fetch("/led?state=" + state)
    .then(response => response.text())
    .then(data => {

      document.getElementById("ledStatus").innerHTML =
        "LED Status: " + data;

    });

}

function updateTemperature() {

  fetch("/temperature")
    .then(response => response.text())
    .then(data => {

      document.getElementById("temperature").innerHTML = data;

    });

}

// Update temperature every 2 seconds
setInterval(updateTemperature, 2000);

// Read immediately
updateTemperature();

</script>

</body>
</html>
)rawliteral";

void handleRoot() {

  server.send(200, "text/html", webpage);

}

void handleLED() {

  if (!server.hasArg("state")) {
    server.send(400, "text/plain", "Missing state");
    return;
  }

  String state = server.arg("state");

  if (state == "ON") {

    digitalWrite(LED_PIN, HIGH);
    server.send(200, "text/plain", "ON");

  }

  else if (state == "OFF") {

    digitalWrite(LED_PIN, LOW);
    server.send(200, "text/plain", "OFF");

  }

  else {

    server.send(400, "text/plain", "Invalid state");

  }

}

void handleTemperature() {

  float temperature = dht.readTemperature();

  if (isnan(temperature)) {

    server.send(500, "text/plain", "Sensor Error");
    return;

  }

  server.send(200, "text/plain", String(temperature, 1));

}

void setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  dht.begin();

  Serial.println();
  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/led", handleLED);
  server.on("/temperature", handleTemperature);

  server.begin();

  Serial.println("Web server started!");

}

void loop() {

  server.handleClient();

}