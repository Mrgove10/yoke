#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>

// Axis
#define PUTCH A0
#define ROLL A1
// Joystick
#define JOY_Y A2
#define JOY_X A3
#define JOY_SWITCH 15
// Right Switchs
#define TOP_SWITCH 0
#define BOT_SWITCH 1
// Center Switchs
#define GEAR_SWITCH 7
#define BATTERY_SWITCH 8
#define AUTO_PILOT_SWITCH 9
#define SPEED A10;
#define FLAP_UP_SWITCH 14
#define FLAP_DOWN_SWITCH 16
#define ENCODER_CLOCK 4
#define ENCODER_COUNTER_CLOCK 5
#define ENCODER_SWITCH 6
//Lcd
//https://learn.adafruit.com/adafruit-grayscale-1-5-128x128-oled-display?view=all
//https://github.com/adafruit/Adafruit_SSD1327/blob/master/examples/ssd1327_test/ssd1327_test.ino
#define OLED_RESET -1
Adafruit_SSD1327 display(128, 128, &Wire, OLED_RESET, 1000000);

void setup()
{
  Serial.begin(115200);
  Serial.println("SSD1327 OLED test");

  if (!display.begin(0x78))
  {
    Serial.println("Unable to initialize OLED");
    while (1)
      yield();
  }
  display.clearDisplay();
  display.display();

  //pinMode(potar, INPUT);
  //pinMode(relay, OUTPUT);
}

void loop()
{
  /*
  lol = analogRead(potar);
  Serial.println(lol);
  digitalWrite(relay, HIGH);
  delay(lol);
  digitalWrite(relay, LOW);
  */
}