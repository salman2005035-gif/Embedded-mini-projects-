#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Pin definitions
#define SDA_PIN 21
#define SCL_PIN 22
#define BUZZER_PIN 25

Adafruit_MPU6050 mpu;

// Movement threshold in radians per second
const float GYRO_THRESHOLD = 0.20;

void setup() {
  Serial.begin(115200);

  // Configure buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Start I2C communication
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not detected!");

    while (1) {
      digitalWrite(BUZZER_PIN, LOW);
      delay(1000);
    }
  }

  Serial.println("Gyroscope ready!");

  // Set sensor ranges
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;

  // Read sensor data
  mpu.getEvent(&a, &g, &temp);

  // Calculate rotational movement magnitude
  float movement = sqrt(
    g.gyro.x * g.gyro.x +
    g.gyro.y * g.gyro.y +
    g.gyro.z * g.gyro.z
  );

  Serial.print("Gyro movement: ");
  Serial.print(movement);
  Serial.println(" rad/s");

  // Check movement threshold
  if (movement > GYRO_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("Movement detected - BUZZER ON");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("Object still - BUZZER OFF");
  }

  delay(50);
}