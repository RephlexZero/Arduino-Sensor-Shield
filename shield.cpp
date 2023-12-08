#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_HIH6130.h>
#include <Adafruit_TSL2572.h>

Adafruit_MMA8451 accelerometer;
Adafruit_HIH6130 humiditySensor;
Adafruit_TSL2572 luminositySensor;

#define LUMINOSITY_SENSOR_PIN A1 // Define the pin connected to the ADC

void setup() {
    pinMode(LUMINOSITY_SENSOR_PIN, INPUT); // Initialize the pin as an input
    Serial.begin(9600); // Initialize serial communication

    accelerometer.begin(); // Initialize the accelerometer
    accelerometer.setRange(MMA8451_RANGE_2_G); // Set the range of the accelerometer to 2G

    humiditySensor.begin(); // Initialize the humidity sensor
}

void loop() {
    int luminosityValue = analogRead(LUMINOSITY_SENSOR_PIN); // Read the luminosity value
    Serial.println(luminosityValue); // Print the luminosity value

    sensors_event_t event;
    accelerometer.getEvent(&event); // Read the accelerometer data

    Serial.print("X: ");
    Serial.print(event.acceleration.x);
    Serial.print(" Y: ");
    Serial.print(event.acceleration.y);
    Serial.print(" Z: ");
    Serial.println(event.acceleration.z);

    // Get magnitude of the accelerometer data
    float magnitude = sqrt(pow(event.acceleration.x, 2) + pow(event.acceleration.y, 2) + pow(event.acceleration.z, 2));
    Serial.print("Magnitude: ");
    Serial.println(magnitude);

    float humidity = humiditySensor.readHumidity(); // Read the humidity value
    float temperature = humiditySensor.readTemperature(); // Read the temperature value

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    delay(500); // Wait for 500 milliseconds before requesting data again
}