#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>
#include <Joystick.h>
#include <Encoder.h>
#include <ArduinoJson.h>

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

// Lcd
// https://learn.adafruit.com/adafruit-grayscale-1-5-128x128-oled-display?view=all
// https://github.com/adafruit/Adafruit_SSD1327/blob/master/examples/ssd1327_test/ssd1327_test.ino
#define LCD_ADDRESS 0x78
#define OLED_RESET -1
Adafruit_SSD1327 display(128, 128, &Wire, OLED_RESET, 1000000);

// joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   12, 0,                // Button Count, Hat Switch Count
                   true, true, false,    // X and Y, but no Z Axis
                   true, true, false,    // No Rx, Ry, or Rz
                   false, true,          // No rudder or throttle
                   false, false, false); // No accelerator, brake, or steering

// Communication
const byte DATA_MAX_SIZE = 128;
char data[DATA_MAX_SIZE]; // an array to store the received data
DynamicJsonDocument doc(DATA_MAX_SIZE);

int seconds = 0;

// https://medium.com/@machadogj/arduino-and-node-js-via-serial-port-bcf9691fab6a
/**
 * parse the incomming data
 * This data is in json format
 */
void parseData()
{
  deserializeJson(doc, data);
  const char *id = doc[0];
  Serial.println(id);
}

/**
 * Receive Data comming from the computer serial connection
 */
void receiveData()
{
  static char endMarker = '\n'; // message separator
  char receivedChar;            // read char from serial port
  int index = 0;                // current index of data buffer

  // clean data buffer
  memset(data, 0, sizeof(data));

  // read while we have data available and we are
  // still receiving the same message.
  while (Serial.available() > 0)
  {
    receivedChar = Serial.read();

    if (receivedChar == endMarker)
    {
      data[index] = '\0'; // end current message

      parseData();
      return;
    }
    else
    {
      // looks like a valid message char, so append it and
      // increment our index
      data[index] = receivedChar;
      index++;
    }

    // if the message is larger than our max size then
    // stop receiving and clear the data buffer.
    if (index >= DATA_MAX_SIZE)
    {
      break;
    }
  }

  // no more available bytes to read from serial and we
  // did not receive the separator. it's an incomplete message!
  Serial.println("KO");
  //Serial.println("error: incomplete message");
  memset(data, 0, sizeof(data));
}

/**
 * Initilise the display
 */
void initializeDisplay()
{
  if (!display.begin(0x3D))
  {
    Serial.println("Unable to initialize OLED");
    while (1)
      yield();
  }
  display.clearDisplay();
  display.display();
}

/**
 * Setup function
 */
void setup()
{
  //Serial
  Serial.begin(115200);

  //LCD
  Serial.println("SSD1327 OLED test");
  initializeDisplay();
 
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

/**
 * Goes on and on and on
 */
void loop()
{
  // Buttons
  Joystick.setButton(1, digitalRead(JOY_SWITCH));
  Joystick.setButton(2, digitalRead(RIGHT_TOP_SWITCH));
  Joystick.setButton(3, digitalRead(RIGHT_BOT_SWITCH));
  Joystick.setButton(4, digitalRead(GEAR_SWITCH));
  Joystick.setButton(5, digitalRead(BATTERY_SWITCH));
  Joystick.setButton(6, digitalRead(AUTO_PILOT_SWITCH));

  // Flaps
  int down = digitalRead(FLAP_DOWN_SWITCH);
  int up = digitalRead(FLAP_UP_SWITCH);

  if (up == 1 && down == 0)
  {
    // Up
    Joystick.setButton(7, 1);
    Joystick.setButton(8, 0);
    Joystick.setButton(9, 0);
  }
  else if (up == 0 && down == 0)
  {
    // Middle
    Joystick.setButton(7, 0);
    Joystick.setButton(8, 1);
    Joystick.setButton(9, 0);
  }
  else if (up == 0 && down == 1)
  {
    //Down
    Joystick.setButton(7, 0);
    Joystick.setButton(8, 0);
    Joystick.setButton(9, 1);
  }

  // Encoder
  Joystick.setButton(10, digitalRead(ENCODER_SWITCH));
  Joystick.setButton(11, 0);
  Joystick.setButton(12, 0);
  long newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {
    if (newPosition > oldPosition)
    {
      // clockwise
      Joystick.setButton(11, 1);
    }
    if (newPosition < oldPosition)
    {
      // counter cloclwise
      Joystick.setButton(12, 1);
    }
    oldPosition = newPosition;
    Serial.println(newPosition);
  }

  // Pots
  Joystick.setXAxis(analogRead(PITCH_POT));
  Joystick.setYAxis(analogRead(ROLL_POT));
  Joystick.setRxAxis(0);   //analogRead(JOY_X));
  Joystick.setRyAxis(0);   //analogRead(JOY_Y));
  Joystick.setThrottle(0); //analogRead(SPEED_POT));

  // Communication
  while (Serial.available() > 0)
  {
    receiveData();
  }

  delay(5);
}