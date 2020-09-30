#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 22
#define DIO 26

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   2000

int cont = 0;
TM1637Display display(CLK, DIO);

const int graaPin = 52;
const int hvitPin = 48;
const int rodPin = 44;
const int blaaPin = 40;

int graaTilstand = 0;
int hvitTilstand = 0;
int rodTilstand = 0;
int blaaTilstand = 0;

int teller = 0;
bool oppdater = true;

bool skift = false;
int ledTeller = 0;
int skiftRegister[] = { -1, -1, -1, -1 };

uint8_t data[] = { 0b00001000, 0b00001000, 0b00001000, 0b00001000 };

void setup() {
  Serial.begin(9600);

  display.setBrightness(0xF);
  display.setSegments(data);
  
  pinMode(graaPin, INPUT);
  pinMode(hvitPin, INPUT);
  pinMode(rodPin, INPUT);
  pinMode(blaaPin, INPUT);
}

void loop() {
  graaTilstand = digitalRead(graaPin);
  hvitTilstand = digitalRead(hvitPin);
  rodTilstand = digitalRead(rodPin);
  blaaTilstand = digitalRead(blaaPin);
  
  if (graaTilstand == HIGH && hvitTilstand == HIGH && rodTilstand == HIGH) 
  {
    if(blaaTilstand == LOW && oppdater == true)
    {
      teller++;
      oppdater = false;
    }
    else
    {
      oppdater = true;
    }

    skift = true;
  }
  else
  {
    if(skift == true)
    {
      if(teller == 10)
      {
        teller = 0;
      }
      
      ledTeller++;
      if(ledTeller == 1)
      {
        skiftRegister[3] = teller;
        data[3] = display.encodeDigit(skiftRegister[3]);
      }
      else if(ledTeller == 2)
      {
        skiftRegister[2] = skiftRegister[3];
        skiftRegister[3] = teller;

        data[2] = display.encodeDigit(skiftRegister[2]);
        data[3] = display.encodeDigit(skiftRegister[3]);
      }
      else if(ledTeller == 3)
      {
        skiftRegister[1] = skiftRegister[2];
        skiftRegister[2] = skiftRegister[3];
        skiftRegister[3] = teller;

        data[1] = display.encodeDigit(skiftRegister[1]);
        data[2] = display.encodeDigit(skiftRegister[2]);
        data[3] = display.encodeDigit(skiftRegister[3]);
      }
      else if(ledTeller >= 4)
      {
        skiftRegister[0] = skiftRegister[1];
        skiftRegister[1] = skiftRegister[2];
        skiftRegister[2] = skiftRegister[3];
        skiftRegister[3] = teller;

        data[0] = display.encodeDigit(skiftRegister[0]);
        data[1] = display.encodeDigit(skiftRegister[1]);
        data[2] = display.encodeDigit(skiftRegister[2]);
        data[3] = display.encodeDigit(skiftRegister[3]);
      }
      
      display.setSegments(data);
      skift = false;
    }
    
    Serial.println(teller);
    delay(250);
    teller = 0;
    oppdater = true;
  }

  delay(50);
}
