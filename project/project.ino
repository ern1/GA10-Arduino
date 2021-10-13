#define FASTLED_INTERNAL
#include <FastLED.h>

// Colors
#define GREEN 128, 0, 0    // CRGB::Green
#define YELLOW 255, 165, 0 // CRGB::Orange
#define RED 0, 255, 0      // CRGB::Red

#define NUM_LEDS 11
#define LED_PIN 3

#define PIEZO_PIN A0
#define PIEZO_THRESHOLD 100

#define TIME_ON 6000 // ms before leds turn off


// Define the array of leds
CRGB leds[NUM_LEDS];
const int num = 32;
int activityHigh = 5, activityMed = 4, activityLow = 2;
int ledBrightness = 255;

// switch and shut off timer
int sensorReading = 0;
int timerOn = TIME_ON;
unsigned long lastTime = 0;


void setLeds(int start, int end, byte r, byte g, byte b) {
  for (int i = start; i < end; i++) {
    leds[i] = CRGB (r, g, b);
  }
  FastLED.show();
}

void setAllLeds(int green, int yellow, int red) {
  int d1 = green, d2 = yellow + green, d3 = red + yellow + green;
  setLeds(0, d1, GREEN);
  setLeds(d1, d2, YELLOW);
  setLeds(d2, d3, RED);
}

// TODO: testa köra med dessa istället (kan ta bort defines för colors om det funkar)
/*
void setLeds(int start, int end, const struct CRGB &color) {
  for (int i = start; i < end; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

void setAllLeds(int green, int yellow, int red) {
  int d1 = green, d2 = yellow + green, d3 = red + yellow + green;
  setLeds(0, d1, CRGB::Green);
  setLeds(d1, d2, CRGB::Orange);
  setLeds(d2, d3, CRGB::Red);
}
*/

// TODO: testa om det funkar att bara köra FastLED.clear() istället för att sätta alla till 0, 0, 0
void setLedsOff() {
  //for (int i = 0; i < NUM_LEDS; i++) {
  //  leds[i] = CRGB (0, 0, 0);
  //}
  FastLED.clear();
  FastLED.show();
}

void printDebugInfo() {
  Serial.print("\tTimer: ");
  Serial.print(timerOn);
  Serial.print("\tLED brightness: ");
  Serial.println(ledBrightness);
}


void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.setBrightness(ledBrightness);

  Serial.begin(9600);
}

void loop() {
  // Read value of piezo switch/sensor
  sensorReading = analogRead(PIEZO_PIN);
  if (sensorReading >= PIEZO_THRESHOLD) {
    timerOn = TIME_ON;
    Serial.println("timerOn reset");
  }

  // Calculate delta time since last iteration
  unsigned long timeNow = millis();
  unsigned long dt = timeNow - lastTime;
  lastTime = timeNow;

  if(timerOn >= 0) {
    //ledBrightness = map(timerOn, 0, TIME_ON, 0, 255);
    ledBrightness = timerOn >= TIME_ON / 2 ? 255 : map(timerOn, 0, TIME_ON / 2, 0, 255);
    FastLED.setBrightness(ledBrightness);
    setAllLeds(activityHigh, activityMed, activityLow);

    timerOn -= (int) dt;
  
  } else {
    setLedsOff();
  }

  printDebugInfo();
  delay(50);
}
