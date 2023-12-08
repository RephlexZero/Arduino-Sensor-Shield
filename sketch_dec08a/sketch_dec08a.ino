#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <HIH61xx.h>
#include <Adafruit_Sensor.h> // Add this line to include the Adafruit Sensor library

Adafruit_MMA8451 accelerometer;
HIH61xx<TwoWire> hih(Wire); // Initialize the humidity sensor with I2C and address

#define TSL257_PIN A1

AsyncDelay samplingInterval;

void powerUpErrorHandler(HIH61xx<TwoWire>& hih)
{
  Serial.println("Error powering up HIH61xx device");
}

void readErrorHandler(HIH61xx<TwoWire>& hih)
{
  Serial.println("Error reading from HIH61xx device");
}

void setup() {
    Wire.begin(); // Initialize I2C communication
    Serial.begin(9600); // Initialize serial communication

    accelerometer.begin(); // Initialize the accelerometer
    accelerometer.setRange(MMA8451_RANGE_2_G); // Set the range of the accelerometer to 2G

    // Set the handlers *before* calling initialise() in case something goes wrong
    hih.setPowerUpErrorHandler(powerUpErrorHandler);
    hih.setReadErrorHandler(readErrorHandler);
    hih.initialise();
    samplingInterval.start(3000, AsyncDelay::MILLIS);
}

void loop() {
    int analogValue = analogRead(A0); // Read the analog value from pin A0
    Serial.println(analogValue); // Print the analog value

    sensors_event_t event;
    accelerometer.getEvent(&event); // Read the accelerometer data

    float magnitude = sqrt(pow(event.acceleration.x, 2) + pow(event.acceleration.y, 2) + pow(event.acceleration.z, 2));
    Serial.print("Magnitude: ");
    Serial.println(magnitude);

    hih.read();

    RED_LED = (magnitude > 1.5) ? 1 : 0;
    ORANGE_LED = (hih.getRelHumidity() / 100.0 > 50) ? 0 : 1;
    GREEN_LED = (analogValue > 512) ? 1 : 0;
    
    // Fetch and print the results
    Serial.print("Relative humidity: ");
    Serial.print(hih.getRelHumidity() / 100.0);
    Serial.println(" %");
    Serial.print("Ambient temperature: ");
    Serial.print(hih.getAmbientTemp() / 100.0);
    Serial.println(" deg C");
    Serial.print("Status: ");
    Serial.println(hih.getStatus());

    samplingInterval.delay(); // Wait for the specified interval before requesting data again
}
