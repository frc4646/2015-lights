#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#define PIN 9
#define PINB 12
#define LENGTH 92  //our actual height is 92

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LENGTH, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB = Adafruit_NeoPixel(LENGTH, PINB, NEO_GRB + NEO_KHZ800);
String inString = "";
extern const uint8_t gamma[];

bool stopStripe = false;
int ledLevel;
int team = 2;
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(10);
  stripB.begin();
  stripB.show(); // Initialize all pixels to 'off'
  stripB.setBrightness(10);

  Serial.begin(9600);
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  team = 2;
}

void loop() {
  strip.setBrightness(127);
  stripB.setBrightness(127);
//  while (!Serial.available())
//  while(!stopStripe)
  while(true)
  {
    theaterChase(strip.Color(pgm_read_byte(&gamma[7]), 255, 0), 100);
//    Iowa flag
//    theaterChase(strip.Color(pgm_read_byte(&gamma[10]), pgm_read_byte(&gamma[31]), pgm_read_byte(&gamma[98])), 100);
//    theaterChase(strip.Color(255, 255, 255), 100);
//    theaterChase(strip.Color(255, 0, 0), 100);
    if (team != 1) { // send 1 over I2C for blue, 0 for red
      theaterChase(strip.Color(0, 0, 200), 100);
    }
    if (team != 0) {
      theaterChase(strip.Color(200, 0, 0), 100);
    }
  }

  while(true)
  {

//  while (Serial.available() > 0) {
//    int inChar = Serial.read();
//    if (isDigit(inChar)) {
//      inString += (char)inChar;
//    }
//    if (inChar == '\n') {
//      Serial.print("Value:");
//      Serial.println(inString.toInt());
//      Serial.print("String: ");
//      Serial.println(inString);
//      int ledLevel = inString.toInt();
//      // clear the string for new input:
//      inString = "";
//    }
//  }
    theaterChaseWithLevel(strip.Color(75, 255, 0), 100, ledLevel);

  }
}

// Fill the dots one after the other with a color
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);
        stripB.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
      stripB.show();
     
      delay(wait);
//      if(stopStripe)
//      {
//        return;
//      }
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);
        stripB.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void setLevel(int level)
{
  for (int i = 0; i<strip.numPixels();i++)
  {
    if (i >= (level-2) && i<=(level+2))
    {
      if (i==level+1 or i==level-1) //1 above and below are orange
      {
        strip.setPixelColor(i, strip.Color(255, 50, 0));
        stripB.setPixelColor(i, stripB.Color(255, 50, 0));
        //strip.setPixelColor(i, strip.Color(0, 170, 0));
      }
      else if (i==level+2 or i==level-2) //2 above and below are red
      {
        strip.setPixelColor(i, strip.Color(255, 0, 0));
        stripB.setPixelColor(i, strip.Color(255, 0, 0));
      }
      else
      {
        strip.setPixelColor(i, strip.Color(0, 255, 0));
        stripB.setPixelColor(i, strip.Color(0, 255, 0)); //the exact one is green
      }
    }
    else
    {
      strip.setPixelColor(i, 0);
      stripB.setPixelColor(i, 0);
    }
  }
  strip.show();
  stripB.show(); 
}

// Fill the dots one after the other with a color
void theaterChaseWithLevel(uint32_t c, uint8_t wait, int level) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      level = ledLevel;
  for (int i = 0; i<strip.numPixels();i++)
  {
    if (i >= (level-4) && i<=(level+4))
    {
      if (i==level+2 || i==level-2) //2 above and below are orange
      {
        strip.setPixelColor(i, strip.Color(255, 50, 0));
        stripB.setPixelColor(i, stripB.Color(255, 50, 0));
        //strip.setPixelColor(i, strip.Color(0, 170, 0));
      }
      else if (i==level+1 || i==level-1) //1 above and below are yellow
      {
        strip.setPixelColor(i, strip.Color(255, 255, 0));
        stripB.setPixelColor(i, strip.Color(255, 255, 0));
      }
      else if (i==level+3 || i==level-3) //3 above and below are red
      {
        strip.setPixelColor(i, strip.Color(255, 0, 0));
        stripB.setPixelColor(i, strip.Color(255, 0, 0));
      }
      else if (i == level + 4 || i == level - 4)
      {
        strip.setPixelColor(i, 0);
        stripB.setPixelColor(i, 0);
      }
      else
      {
        strip.setPixelColor(i, strip.Color(0, 255, 0));
        stripB.setPixelColor(i, strip.Color(0, 255, 0)); //the exact one is green
      }
    }
    else
    {
      if(i % 3 == q)
      {
        strip.setPixelColor(i, c);
        stripB.setPixelColor(i, c);    //turn every third pixel on
      }
      else
      {
            strip.setPixelColor(i, 0);
            stripB.setPixelColor(i, 0);
      }
    }
  }
  strip.show();
  stripB.show(); 
     
      delay(wait);
    }
  }
}



void receiveEvent(int howMany)
{
  stopStripe = true;
  while(Wire.available()>0)
  {
//   ledLevel = Wire.read();
  team = Wire.read();
  }
}

const uint8_t PROGMEM gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
