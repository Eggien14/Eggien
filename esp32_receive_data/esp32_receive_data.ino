#include <HardwareSerial.h>


unsigned long previousMillis = 0;
const int interval = 1000;

HardwareSerial pointA(1);
HardwareSerial pointB(2);

void setup() {
  pointA.begin(9600, SERIAL_8N1, 3, 1);
  pointB.begin(9600, SERIAL_8N1, 16, 17);
  Serial.begin(9600);
  Serial.println("Initializing...");
}

void loop() {
  // send();
  receive();
  delay(10);
}
// void send() {
//   unsigned long currentMillis = millis(); 
//   if (currentMillis - previousMillis >= interval) {
   
//     previousMillis = currentMillis;

//     // send sample data
//     pointB.println("Ax");
//     Serial.println("Send data: Ax");
//   }
// }
void receive() {
  while (pointA.available() > 0) {
    String data = pointA.readStringUntil('x'); 
    //print received data
    Serial.print("Received data: ");
    Serial.println(data);
  }
}