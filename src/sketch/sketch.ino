#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 18, 4);

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     -3 * 3600 // GMT -3
#define UTC_OFFSET_DST 0

#define WIFI_USERNAME "IoT"
#define WIFI_PASSWORD "NcC-1701"

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
  printOnline();
  LCD.setCursor(0, 0);
  LCD.print("Updating time...  ");
}

void printOnline() {
  Serial.println("printOnline");

  LCD.setCursor(0, 3);
  LCD.print("Online");
}

void printOffLine() {
  Serial.println("printOffLine");

  LCD.setCursor(0, 3);
  LCD.print("Offline");
}

void printLocalTime() {
  Serial.println("printLocalTime");

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    printOffLine();
    return;
  }

  LCD.setCursor(0, 0);
  LCD.print(&timeinfo, "%d/%m/%Y  %H:%M:%S");

  printOnline();
}

void printTemperature() {
 Serial.println("printTemperature");
}

void setup() {
  Serial.begin(115200);

  Serial.println("setup");

  LCD.init();
  LCD.backlight();

  connectToWifi();

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

void loop() {
  printLocalTime();
  printTemperature();
  delay(250);
}
