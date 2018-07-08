// Chapter 7 - Communications
// I2C Master
// By Cornel Amariei for Packt Publishing

// Include the required Wire library for I2C
#include <Wire.h>
#include <Adafruit_FeatherOLED.h>

int x = 0;
int led = 13;

Adafruit_FeatherOLED oled = Adafruit_FeatherOLED();
void setup() {
  // Start the I2C Bus as Master
  Wire.begin(); 
  oled.init();
  pinMode(led, OUTPUT);
  Serial.begin(115200);

}

void loop() {
  oled.clearMsgArea();
  Wire.requestFrom(0, 32);
  Serial.println("Request made");
 
  while(Wire.available()){
    char c = Wire.read();
    oled.print(c);
    Serial.print(c);
  }
  Serial.println();
  oled.display();
  delay(1000);

}

