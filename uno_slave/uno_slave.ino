// Chapter 7 - Communications
// I2C Slave
// By Cornel Amariei for Packt Publishing

// Include the required Wire library for I2C
#include <Wire.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

int LED = 13;


SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO true

char *globalstr = "Waiting.. ";

boolean usingInterrupt = true;
void useInterrupt(boolean);

void setup() {
  Wire.begin(0);
  Wire.onRequest(requestEvent);

  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  GPS.sendCommand(PGCMD_NOANTENNA);
  
  useInterrupt(true);

}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;
  // writing direct to UDR0 is much much faster than Serial.print
  // but only one character can be written at a time.
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

void requestEvent() {
  //Serial.println("*****GOT REQUEST*****");  
  Wire.write(globalstr);
  Serial.print("Sending "); Serial.println(globalstr);
}


void loop() {

   if (GPS.newNMEAreceived()) {
    char *stringptr = GPS.lastNMEA();
    
    if (!GPS.parse(stringptr))
      return;
    //Serial.println("OK");
    if(GPS.fix){
      Serial.print("Fix");
      strcpy(globalstr, stringptr);
      Serial.print("UPDATED GLOBAL STR");
      //Serial.println(strlen(globalstr));
    }
  }

}



