#include <Adafruit_NeoPixel.h>

#define PIN 12
int NUM_LEDS = 200;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);
bool newEffect = false;
int currentCase = 1;
int oldCase = 1;
bool clearStripBeforehand = false;



void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  setAll(0, 0, 0);
  delay(7000);
//  millisTrack = millis();
  strip.setBrightness(255);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (clearStripBeforehand) {
    clearStrip();
    clearStripBeforehand = false;
   }
    handleCase(currentCase);
}
