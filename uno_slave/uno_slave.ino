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
#define GPSECHO false


boolean usingInterrupt = true;
void useInterrupt(boolean);

struct timedata {
  int day;
  int month;
  int year;
  int hour;
  int mit;
  int sec;
};
char global[] = {0,0,0,0,0,0};

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
  Serial.print("Sending ");
  Serial.println(Wire.write(global, 24));
  Serial.println((int) global[1]);
  //Serial.println(globalstr);
}


void loop() {

  if (GPS.newNMEAreceived()) {
    //Serial.println("recv");
    char *stringptr = GPS.lastNMEA();
    if (GPS.parse(stringptr)) {
      int hour = GPS.hour;
      int sec = GPS.seconds;
      int mit = GPS.minute;
      int day = GPS.day;
      int mon = GPS.month;
      int year = GPS.year;

      char local[] = {sec, mit, hour, day, mon, year};
      memcpy(local, global, 24);      
      Serial.println("Parsed");
      Serial.println(mit);
    }
    else{
      Serial.println("Dicked up");
    }
  }
}



