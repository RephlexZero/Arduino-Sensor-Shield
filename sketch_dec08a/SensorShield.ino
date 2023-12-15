#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <HIH61xx.h>
#include <Adafruit_Sensor.h>

#define RED_LED 10
#define ORANGE_LED 11
#define GREEN_LED 12

Adafruit_MMA8451 accelerometer;
HIH61xx<TwoWire> hih(Wire); // Initialize the humidity sensor with I2C and address

#define TSL257_PIN A1

AsyncDelay samplingInterval;

static float humidity;
static int luminosity;
static uint8_t orientation;

void powerUpErrorHandler(HIH61xx<TwoWire> &hih)
{
  Serial.println("Error powering up HIH61xx device");
}

void readErrorHandler(HIH61xx<TwoWire> &hih)
{
  Serial.println("Error reading from HIH61xx device");
}

void setup()
{
  pinMode(RED_LED, OUTPUT);
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  Wire.begin();       // Initialize I2C communication
  Serial.begin(9600); // Initialize serial communication

  accelerometer.begin();                     // Initialize the accelerometer
  accelerometer.setRange(MMA8451_RANGE_2_G); // Set the range of the accelerometer to 2G

  // Set the handlers *before* calling initialise() in case something goes wrong
  hih.setPowerUpErrorHandler(powerUpErrorHandler);
  hih.setReadErrorHandler(readErrorHandler);
  hih.initialise();
  samplingInterval.start(3000, AsyncDelay::MILLIS);
}

void loop()
{
  int luminosity = analogRead(TSL257_PIN); // Read the analog value from the light sensor
  Serial.print("Luminosity:");
  Serial.println(luminosity); // Print the analog value

  sensors_event_t event;
  accelerometer.getEvent(&event); // Read the accelerometer data

  float magnitude = sqrt(pow(event.acceleration.x, 2) + pow(event.acceleration.y, 2) + pow(event.acceleration.z, 2));
  Serial.print("Magnitude:");
  Serial.println(magnitude);

  orientation = accelerometer.getOrientation();

  hih.read();

  // Fetch and print the results
  Serial.print("Relative humidity:");
  humidity = hih.getRelHumidity() / 100.0;
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Ambient temperature:");
  Serial.print(hih.getAmbientTemp() / 100.0);
  Serial.println(" deg C");

  digitalWrite(RED_LED, (magnitude > 1.5));
  digitalWrite(ORANGE_LED, (humidity > 55));
  //digitalWrite(GREEN_LED, (luminosity > 300));
  digitalWrite(GREEN_LED, orientation == MMA8451_PL_PUF);
  switch (orientation)
  {
  case MMA8451_PL_PUF:
    Serial.println("Portrait Up Front");
    break;
  case MMA8451_PL_PUB:
    Serial.println("Portrait Up Back");
    break;
  case MMA8451_PL_PDF:
    Serial.println("Portrait Down Front");
    break;
  case MMA8451_PL_PDB:
    Serial.println("Portrait Down Back");
    break;
  case MMA8451_PL_LRF:
    Serial.println("Landscape Right Front");
    break;
  case MMA8451_PL_LRB:
    Serial.println("Landscape Right Back");
    break;
  case MMA8451_PL_LLF:
    Serial.println("Landscape Left Front");
    break;
  case MMA8451_PL_LLB:
    Serial.println("Landscape Left Back");
    break;
  }
}
