#include <Arduino.h>

int lol;
int potar = A0;
int relay = D0;

void setup()
{

  pinMode(potar, INPUT);
  pinMode(relay, OUTPUT);
  Serial.begin(115200);
}

void loop() // run over and over
{
  lol = analogRead(potar);
  Serial.println(lol);
  digitalWrite(relay, HIGH);
  delay(lol);
  digitalWrite(relay, LOW);
}