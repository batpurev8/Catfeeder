#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6_J6oZTqQ"
#define BLYNK_TEMPLATE_NAME "Batpurev"
#define BLYNK_AUTH_TOKEN "BFbfjxF6N2HDIGnr_OXNYrm0IVctb5za"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Servo.h>

// WiFi credentials
const char *ssid = "iptime";
const char *password = "";
const char *auth = "BFbfjxF6N2HDIGnr_OXNYrm0IVctb5za";

// NTP Client setup
const long offsetTime = 32400; 
String days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", offsetTime);

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
WidgetLCD virtualLCD(V1);

// Servo setup
Servo servo;
int lastPosition = 0;
int onnPos = 2000;  
int offPos = -2000; 

bool displayTime = true;
// LCD display
BLYNK_WRITE(V1) {
  displayTime = param.asInt();
}
// Servo Motor Blynk
BLYNK_WRITE(V0) {
  int value = param.asInt();
  Serial.println(value);

  if (value == 1 && lastPosition != onnPos) {
    servo.attach(D4);
    servo.writeMicroseconds(onnPos);
    delay(500);
    servo.detach();
    lastPosition = onnPos;
  } else if (value == 0 && lastPosition != offPos) {
    servo.attach(D4);
    servo.writeMicroseconds(offPos);
    delay(500);
    servo.detach();
    lastPosition = offPos;
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, password);

  // Initialize NTP Client
  timeClient.begin();

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Time is ready..");
  virtualLCD.print(0, 0, "Time is ready..");
  delay(3000);

  lcd.clear();
  virtualLCD.clear();
}

void loop() {
  Blynk.run();
  if (displayTime) {
    updateTime();
  }
}

void updateTime() {
  timeClient.update();

  String currentDay = days[timeClient.getDay()];
  String currentTime = timeClient.getFormattedTime();

  Serial.print("Today: ");
  Serial.println(currentDay);
  Serial.print("Time: ");
  Serial.println(currentTime);

  lcd.setCursor(0, 0);
  lcd.print("Today: ");
  lcd.print(currentDay);

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(currentTime);

  virtualLCD.print(0, 0, "Today: ");
  virtualLCD.print(6, 0, currentDay);
  virtualLCD.print(0, 1, "Time: ");
  virtualLCD.print(5, 1, currentTime);

  delay(800);
}
