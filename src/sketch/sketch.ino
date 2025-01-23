// Learn about the ESP32 WiFi simulation in
// https://docs.wokwi.com/guides/esp32-wifi
// https://wokwi.com/projects/321525495180034642

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 18, 4);

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     -3 * 3600
#define UTC_OFFSET_DST 0

void spinner() {
  static int8_t counter = 0;
  const char* glyphs = "\xa1\xa5\xdb";
  LCD.setCursor(18, 0);
  LCD.print(glyphs[counter++]);
  if (counter == strlen(glyphs)) {
    counter = 0;
  }
}

void printOnline() {
  LCD.setCursor(0, 3);
  LCD.print("Online");
}

void printOffLine() {
  LCD.setCursor(0, 3);
  LCD.print("Offline");
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    printOffLine();
    return;
  }

  LCD.setCursor(0, 0);
  LCD.print(&timeinfo, "%d/%m/%Y  %H:%M:%S");
  //LCD.print(&timeinfo, "%d/%m/%Y       %Z");

  //LCD.setCursor(0, 1);
  //LCD.print(&timeinfo, "%H:%M:%S            ");

  printOnline();
}

void setup() {
  Serial.begin(115200);

  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("Connecting to WiFi");

  WiFi.begin("Go_To_Hell", "4a5l8a6n1", 6);
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

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

void loop() {
  printLocalTime();
  delay(250);
}
