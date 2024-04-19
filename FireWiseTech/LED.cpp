#include <Arduino.h>

void setup() 
{
  Serial.begin(115200);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
}

void loop() 
{
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
  delay(5000);
  digitalWrite(22,LOW);
  digitalWrite(23, HIGH);
  delay(5000);
}
