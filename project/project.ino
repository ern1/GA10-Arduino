#define FASTLED_INTERNAL
#include <FastLED.h>


#define NUM_LEDS 11
#define LED_PIN 3
#define MAX_LED_BRIGHTNESS 255

#define PIEZO_PIN A0
#define PIEZO_THRESHOLD 130

#define TIME_ON 10000 // ms before leds turn off


// Define the array of leds
CRGB leds[NUM_LEDS];
const int num = 32;
int activityHigh = 5, activityMed = 4, activityLow = 2;
int ledBrightness = MAX_LED_BRIGHTNESS;

// switch and shut off timer
int sensorReading = 0;
int timerOn = TIME_ON;
unsigned long lastTime = 0;


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

void setLedsOff() {
  FastLED.clear();
  FastLED.show();
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    Serial.println(inChar);

    switch(inChar) {
      case 'q':
        if (activityHigh == 0) break;
        activityHigh--; activityMed++;
        break;
      case 'w':
        if (activityMed == 0) break;
        activityHigh++; activityMed--;
        break;
      case 'e':
        if (activityMed == 0) break;
        activityLow++; activityMed--;
        break;
      case 'r':
        if (activityLow == 0) break;
        activityMed++; activityLow--;
        break;
    }
  }
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
    ledBrightness = (timerOn >= TIME_ON / 3) ?
        MAX_LED_BRIGHTNESS : map(timerOn, 0, TIME_ON / 3, 16, MAX_LED_BRIGHTNESS);
    FastLED.setBrightness(ledBrightness);
    setAllLeds(activityHigh, activityMed, activityLow);

    //timerOn -= (int) dt;
  
  } else {
    setLedsOff();
  }

  //printDebugInfo();
  delay(50);
}
