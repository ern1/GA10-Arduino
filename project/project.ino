#define FASTLED_INTERNAL
#include <FastLED.h>
//#include <SoftwareSerial.h>

// colors (fast kan säkert använda macron som redan finns i FastLED.h...)
#define GREEN 255, 0, 0
#define YELLOW 255, 128, 0
#define RED 0, 255, 0

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
int lastTime = 0;

SoftwareSerial mySerial(2, 3);


void setLeds(int start, int end, byte r, byte g, byte b) {
  for (int i = start; i < end; i++) {
    leds[i] = CRGB (r, g, b);
  }
  FastLED.show();
}

void setLedsOff() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB (0, 0, 0);
  }
  FastLED.show();
  //FastLED.shut(); // Räcker kanske med att enbart köra denna istället 
}

void setAllLeds(int green, int yellow, int red) {
  int d1 = green, d2 = yellow + green, d3 = red + yellow + green;
  setLeds(0, d1, GREEN);
  setLeds(d1, d2, YELLOW);
  setLeds(d2, d3, RED);
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

  // calculate delta time since last iteration
  int timeNow = millis();
  int dt = timeNow - lastTime;
  lastTime = timeNow;

  if(timerOn > 0) {
    ledBrightness = map(timerOn, 0, TIME_ON, 0, 255);
    setAllLeds(activityHigh, activityMed, activityLow);
    FastLED.setBrightness(ledBrightness);
    timerOn -= dt;
  } else {
    setLedsOff();
  }

  delay(50);

}
