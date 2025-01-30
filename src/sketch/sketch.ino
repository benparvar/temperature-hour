#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h>

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     -3 * 3600 // GMT -3
#define UTC_OFFSET_DST 0

#define WIFI_USERNAME "IoT"
#define WIFI_PASSWORD "NcC-1701"

#define I2C_FREQ 400000

#define SDA_LCD 21
#define SCL_LCD 22
#define SDA_SENSOR 33
#define SCL_SENSOR 32

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 18, 4); // LCD Display
Adafruit_BMP085 BMP; // Sensor
bool isSensorAvailable = false;

void spinner() {
  static int8_t counter = 0;
  const char* glyphs = "\xa1\xa5\xdb";
  LCD.setCursor(18, 0);
  LCD.print(glyphs[counter++]);
  if (counter == strlen(glyphs)) {
    counter = 0;
  }
}

void connectToWifi() {
  Serial.println("connectToWifi");

  LCD.setCursor(0, 0);
  LCD.print("Connecting to WiFi");

  WiFi.begin(WIFI_USERNAME, WIFI_PASSWORD, 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    spinner();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Updating time...  ");
}

void printLocalTime() {
  Serial.println("printLocalTime");

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }

  LCD.setCursor(0, 0);
  LCD.print(&timeinfo, "%d/%m/%Y  %H:%M:%S");
}

void printAltitude() {
  if (isSensorAvailable) {
    Serial.println("printAltitude");
  
    LCD.setCursor(0, 3);
    LCD.print("Alt: ");
    LCD.print(BMP.readAltitude());
    LCD.print(" Mt");
  }
}

void printPressure() {
  if (isSensorAvailable) {
    Serial.println("printPressure");
  
    LCD.setCursor(0, 2);
    LCD.print("Pres: ");
    LCD.print(BMP.readPressure());
    LCD.print(" Pa");
  }
}

void printTemperature() {
  if (isSensorAvailable) {
    Serial.println("printTemperature");
  
    LCD.setCursor(0, 1);
    LCD.print("Temp: ");
    LCD.print(BMP.readTemperature());
    LCD.print(" *C");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  // Configure Wire
  Wire.begin();
  Wire1.begin(SDA_SENSOR, SCL_SENSOR);

  // Configure LCD
  LCD.init();
  LCD.backlight();

  // Configure wifi
  connectToWifi();


  // Configure Sensor
  isSensorAvailable = BMP.begin(0x76, &Wire1);
  if (!isSensorAvailable) {
    Serial.println("BMP sensor fail...");
  }

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

void loop() {
  printLocalTime();
  printTemperature();
  printPressure();
  printAltitude();
  delay(500);
}
