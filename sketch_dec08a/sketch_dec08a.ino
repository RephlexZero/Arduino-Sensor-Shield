#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_HIH6130.h>

Adafruit_MMA8451 accelerometer;
Adafruit_HIH6130 humiditySensor;

#define TSL257_PIN A1

void setup() {
    Wire.begin(); // Initialize I2C communication
    Serial.begin(9600); // Initialize serial communication

    accelerometer.begin(); // Initialize the accelerometer
    accelerometer.setRange(MMA8451_RANGE_2_G); // Set the range of the accelerometer to 2G

    humiditySensor.begin(); // Initialize the humidity sensor
}

void loop() {
    Wire.requestFrom(8, 2); // Request 2 bytes of data from slave device with address 8

    int analogValue = analogRead(A0); // Read the analog value from pin A0
    Serial.println(analogValue); // Print the analog value

    sensors_event_t event;
    accelerometer.getEvent(&event); // Read the accelerometer data

    Serial.print("X: ");
    Serial.print(event.acceleration.x);
    Serial.print(" Y: ");
    Serial.print(event.acceleration.y);
    Serial.print(" Z: ");
    Serial.println(event.acceleration.z);

    float humidity = humiditySensor.readHumidity(); // Read the humidity value
    float temperature = humiditySensor.readTemperature(); // Read the temperature value

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    delay(500); // Wait for 500 milliseconds before requesting data again
}
