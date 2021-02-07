#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>
#include <Joystick.h>
#include <Encoder.h>

// Axis
#define PITCH_POT A0
#define ROLL_POT A1

// Joystick
#define JOY_Y A2
#define JOY_X A3
#define JOY_SWITCH 15

// Right Switchs
#define RIGHT_TOP_SWITCH 5
#define RIGHT_BOT_SWITCH 6

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
                   15, 0,                // Button Count, Hat Switch Count
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
  pinMode(RIGHT_TOP_SWITCH, INPUT_PULLUP);
  pinMode(RIGHT_BOT_SWITCH, INPUT_PULLUP);

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
  // Buttons
  Joystick.setButton(0, digitalRead(JOY_SWITCH));
  Joystick.setButton(1, digitalRead(RIGHT_TOP_SWITCH));
  Joystick.setButton(2, digitalRead(RIGHT_BOT_SWITCH));
  Joystick.setButton(3, digitalRead(GEAR_SWITCH));
  Joystick.setButton(4, digitalRead(BATTERY_SWITCH));
  Joystick.setButton(5, digitalRead(AUTO_PILOT_SWITCH));

  // Flaps
  int down = digitalRead(FLAP_DOWN_SWITCH);
  int up = digitalRead(FLAP_UP_SWITCH);

  if (up == 1 && down == 0)
  {
    // Up
    Joystick.setButton(6, 1);
    Joystick.setButton(7, 0);
    Joystick.setButton(8, 0);
  }
  else if (up == 0 && down == 0)
  {
    // Middle
    Joystick.setButton(6, 0);
    Joystick.setButton(7, 1);
    Joystick.setButton(8, 0);
  }
  else if (up == 0 && down == 1)
  {
    //Down
    Joystick.setButton(6, 0);
    Joystick.setButton(7, 0);
    Joystick.setButton(8, 1);
  }

  // Encoder
  Joystick.setButton(9, digitalRead(ENCODER_SWITCH));
  Joystick.setButton(10, 0);
  Joystick.setButton(11, 0);
  long newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {
    if (newPosition > oldPosition)
    {
      // clockwise
      Joystick.setButton(10, 1);
    }
    if (newPosition < oldPosition)
    {
      // counter cloclwise

      Joystick.setButton(11, 1);
    }
    oldPosition = newPosition;
    Serial.println(newPosition);
  }

  // Pots
  Joystick.setXAxis(analogRead(PITCH_POT));
  Joystick.setYAxis(analogRead(ROLL_POT));
  Joystick.setRxAxis(analogRead(JOY_X));
  Joystick.setRyAxis(analogRead(JOY_Y));
  Joystick.setThrottle(analogRead(SPEED_POT));

  delay(5);
}