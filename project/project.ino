#define FASTLED_INTERNAL
#include <FastLED.h>

/* LEDs side (ring) */
#define NUM_LEDS 11
#define LED_PIN 3

// Activity
#define MAX_LED_BRIGHTNESS 255
#define TIME_ON 6000 // ms before leds turn off

// Pulse/notification
#define PULSE_OFF 0
#define PULSE_SHORT 1
#define PULSE_LONG 2
#define TIME_PULSE_TOGGLE_SHORT 250
#define TIME_PULSE_TOGGLE_LONG 800
#define PULSE_ITER_SHORT 4
#define PULSE_ITER_LONG 10
#define MIN_PULSE_LED_BRIGHTNESS 16
#define MAX_PULSE_LED_BRIGHTNESS 128

/* LEDs front (for the tree) */
#define NUM_LEDS_FRONT 4
#define LED_PIN_FRONT 5

/* Piezo button */
#define PIEZO_PIN A0
#define PIEZO_THRESHOLD 130


// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB ledsFront[NUM_LEDS_FRONT];

int activityHigh = 5, activityMed = 4, activityLow = 2;
int ledBrightness = MAX_LED_BRIGHTNESS;

// For pulse sequence
int pulseType = PULSE_OFF, pulseLedOn, timerToggle, iterRemaining;

// switch and shut off timer
int sensorReading = 0;
int timerOn = TIME_ON;
unsigned long dt, lastTime = 0;


void setLeds(int start, int end, const struct CRGB &color) {
  for (int i = start; i < end; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

void setLedsFront(const struct CRGB &color) {
  for (int i = 0; i < NUM_LEDS_FRONT; i++) {
    ledsFront[i] = color;
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

void setPulse(int type) {
  if(type == PULSE_SHORT) {
    pulseLedOn = 1; timerToggle = TIME_PULSE_TOGGLE_SHORT; iterRemaining = PULSE_ITER_SHORT;
  } else if (type == PULSE_LONG) {
    pulseLedOn = 1; timerToggle = TIME_PULSE_TOGGLE_LONG; iterRemaining = PULSE_ITER_LONG;
  } else {
    pulseType = PULSE_OFF;
    return;
  }

  pulseType = type;
}

void pulseLeds() {
  const int timeToggle = pulseType == PULSE_SHORT ? TIME_PULSE_TOGGLE_SHORT : TIME_PULSE_TOGGLE_LONG;

  if (timerToggle <= 0) {
    pulseLedOn *= -1;
    timerToggle = timeToggle;
    iterRemaining -= 1;
  }

  if (iterRemaining <= 0)
    pulseType = PULSE_OFF;

  FastLED.setBrightness(pulseLedOn == 1
                          ? map(timerToggle, 0, timeToggle,
                                MAX_PULSE_LED_BRIGHTNESS, MIN_PULSE_LED_BRIGHTNESS)
                          : map(timerToggle, 0, timeToggle,
                                MIN_PULSE_LED_BRIGHTNESS, MAX_PULSE_LED_BRIGHTNESS));
  setLeds(0, NUM_LEDS, CRGB::Red);
  setLedsFront(CRGB::Black);

  timerToggle -= dt;
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
      case 'o':
        setPulse(PULSE_SHORT);
        break;
      case 'p':
        setPulse(PULSE_LONG);
        break;
      case 'l':
        setPulse(PULSE_OFF);
        break;
      case 't':
        timerOn = TIME_ON;
        break;
    }
  }
}

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN_FRONT>(ledsFront, NUM_LEDS);
  FastLED.setBrightness(ledBrightness);
  setLedsFront(CRGB::Orange);

  Serial.begin(9600);
}

void loop() {
  // Calculate delta time (dt) since last iteration
  unsigned long timeNow = millis();
  dt = timeNow - lastTime;
  lastTime = timeNow;

  // Read value of piezo switch/sensor
  sensorReading = analogRead(PIEZO_PIN);
  if (sensorReading >= PIEZO_THRESHOLD) {
    //timerOn = TIME_ON;
    setPulse(PULSE_SHORT);
    Serial.println("tap");
  }

  // Set LEDs
  if(pulseType != PULSE_OFF) {
    pulseLeds();
  } else if (timerOn >= 0) {
    // If timer is below a certain threshold, gradually lower the brightness
    ledBrightness = (timerOn >= TIME_ON / 3)
                      ? MAX_LED_BRIGHTNESS
                      : map(timerOn, 0, TIME_ON / 3, 16, MAX_LED_BRIGHTNESS);

    FastLED.setBrightness(ledBrightness);
    setAllLeds(activityHigh, activityMed, activityLow);

    timerOn -= (int) dt;
  } else {
    setLedsOff();
  }

  delay(10); // TODO: remove?
}
