#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>
#include <Joystick.h>

// Axis
#define PITCH_POT A0
#define ROLL_POT A1

// Joystick
#define JOY_Y A2
#define JOY_X A3
#define JOY_SWITCH 15

// Right Switchs
#define TOP_SWITCH 5
#define BOT_SWITCH 6

// Center Switchs
#define GEAR_SWITCH 7
#define BATTERY_SWITCH 8
#define AUTO_PILOT_SWITCH 9
#define SPEED_POT A10
#define FLAP_UP_SWITCH 14
#define FLAP_DOWN_SWITCH 16
// Encoder
#define ENCODER_CLOCK 0
#define ENCODER_COUNTER_CLOCK 1
#define ENCODER_SWITCH 6
Encoder myEnc(ENCODER_CLOCK, ENCODER_COUNTER_CLOCK);
long oldPosition = -999;

//Lcd
//https://learn.adafruit.com/adafruit-grayscale-1-5-128x128-oled-display?view=all
//https://github.com/adafruit/Adafruit_SSD1327/blob/master/examples/ssd1327_test/ssd1327_test.ino
#define LCD_ADDRESS 0x78
#define OLED_RESET -1
Adafruit_SSD1327 display(128, 128, &Wire, OLED_RESET, 1000000);

//joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   3, 0,                 // Button Count, Hat Switch Count
                   true, true, false,    // X and Y, but no Z Axis
                   true, true, false,    // No Rx, Ry, or Rz
                   false, true,          // No rudder or throttle
                   false, false, false); // No accelerator, brake, or steering

void setup()
{
  //Serial
  Serial.begin(115200);

  //LCD
  Serial.println("SSD1327 OLED test");
  /*if (!display.begin(LCD_ADDRESS))
  {
    Serial.println("Unable to initialize OLED");
    while (1)
      yield();
  }*/
  display.clearDisplay();
  //display.display();

  // Joystick
  Joystick.begin();
  Joystick.setXAxisRange(0, 1024);
  Joystick.setYAxisRange(0, 1024);
  Joystick.setRxAxisRange(0, 1024);
  Joystick.setRyAxisRange(0, 1024);
  Joystick.setThrottleRange(0, 1024);

  // PinMode
  // Axis
  pinMode(PITCH_POT, INPUT);
  pinMode(ROLL_POT, INPUT);

  // Joystick
  pinMode(JOY_Y, INPUT);
  pinMode(JOY_X, INPUT);
  pinMode(JOY_SWITCH, INPUT_PULLUP);

  // Right Switchs
  pinMode(TOP_SWITCH, INPUT_PULLUP);
  pinMode(BOT_SWITCH, INPUT_PULLUP);

  // Center Switchs
  pinMode(GEAR_SWITCH, INPUT_PULLUP);
  pinMode(BATTERY_SWITCH, INPUT_PULLUP);
  pinMode(AUTO_PILOT_SWITCH, INPUT_PULLUP);
  pinMode(SPEED_POT, INPUT);
  pinMode(FLAP_UP_SWITCH, INPUT_PULLUP);
  pinMode(FLAP_DOWN_SWITCH, INPUT_PULLUP);

  //Encoder
  pinMode(ENCODER_CLOCK, INPUT);
  pinMode(ENCODER_COUNTER_CLOCK, INPUT);
  pinMode(ENCODER_SWITCH, INPUT_PULLUP);
}

void loop()
{
  long newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }

  for (size_t i = 0; i < 1024; i++)
  {
    Joystick.setXAxis(i);
    Joystick.setYAxis(i);
    Joystick.setRxAxis(i);
    Joystick.setRyAxis(i);
    Joystick.setThrottle(i);
  }

  Joystick.setButton(0, 0);
  delay(500);
  Joystick.setButton(0, 1);
}