#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>
#include <ArduinoJson.h>

// Lcd
// https://learn.adafruit.com/adafruit-grayscale-1-5-128x128-oled-display?view=all
// https://github.com/adafruit/Adafruit_SSD1327/blob/master/examples/ssd1327_test/ssd1327_test.ino
#define LCD_ADDRESS 0x78
#define OLED_RESET -1
Adafruit_SSD1327 display(128, 128, &Wire, OLED_RESET, 1000000);


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
  if (!display.begin(0x3C))
  {
    Serial.println("Unable to initialize OLED");
    while (1)
      yield();
  }
  display.clearDisplay();
  display.setRotation(2);
}

/**
 * Setup function
 */
void setup()
{
  //Serial
  Serial.begin(115200);

  //LCD
  initializeDisplay();
 
  display.drawPixel(0,0,255);
  display.setCursor(0,10);
  display.drawChar('test');
  display.display();
}

/**
 * Goes on and on and on
 */
void loop()
{
  // Communication
  while (Serial.available() > 0)
  {
    receiveData();
  }

  delay(5);
}