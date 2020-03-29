
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "Switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include <Adafruit_NeoPixel.h>


//LED Stuff

#define PIN 14
int NUM_LEDS = 200;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);
bool newEffect = false;
int currentCase = 25;
int oldCase = 25;
bool neverChanged = true;


// prototypes
boolean connectWifi();
bool wifiConnected = false;

//on/off callbacks
void lightOn();
void lightOff();
void secondOn();
void secondOff();

//------- Replace the following! ------
//char ssid[] = "UofM-Guest";       // your network SSID (name)
//char password[] = "";  // your network key
//const char* ssid = "Brady's iPhone 6";
//const char* password = "brady151";
const String ssid = "StoffelNetwork-5";
const String password = "9529471171";

UpnpBroadcastResponder wemoManager;
Switch *rainbowEffect = NULL;
Switch *coolEffect = NULL;
Switch *warmEffect = NULL;
Switch *natureEffect = NULL;
Switch *candyEffect = NULL;
Switch *christmasEffect = NULL;

Switch *lightingEffect = NULL;
Switch *MedHighLightingEffect = NULL;
Switch *MedLightingEffect = NULL;
Switch *MedLowLightingEffect = NULL;


const int ledPin = LED_BUILTIN;

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  setAll(0, 0, 0);





// Uncomment this is WEMO ever works for Alexa again
// Doesn't work as of now which is disappointing
//  setupWemoStuff();




  setupLights();

  //  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  //  delay(10);
  //  digitalWrite(ledPin, HIGH); // Wemos BUILTIN_LED is active Low, so high is off
}

void setupWemoStuff() {

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  wifiConnected = true;
  
  wemoManager.beginUdpMulticast();

  // Format: Alexa invocation name, local port no, on callback, off callback
  rainbowEffect = new Switch("rainbow effect", 80, rainbowLightsOn, lightsOff);
  coolEffect = new Switch("cool effect", 81, coolLightsOn, lightsOff);
  warmEffect = new Switch("warm effect", 82, warmLightsOn, lightsOff);
  natureEffect = new Switch("nature effect", 83, natureLightsOn, lightsOff);
  candyEffect = new Switch("candy effect", 84, candyLightsOn, lightsOff);
  christmasEffect = new Switch("christmas effect", 85, christmasLightsOn, lightsOff);

  lightingEffect = new Switch("white lighting", 84, lightsOn, lightsOff);

  MedHighLightingEffect = new Switch("Medium High lighting", 87, mediumHighLightsOn, lightsOff);
  MedLightingEffect = new Switch("Medium lighting", 88, mediumLightsOn, lightsOff);
  MedLowLightingEffect = new Switch("Medium Low lighting", 89, mediumLowLightsOn, lightsOff);

  wemoManager.addDevice(*rainbowEffect);
  wemoManager.addDevice(*lightingEffect);

  wemoManager.addDevice(*coolEffect);
  wemoManager.addDevice(*warmEffect);
  wemoManager.addDevice(*natureEffect);
  wemoManager.addDevice(*candyEffect);
  wemoManager.addDevice(*christmasEffect);

  wemoManager.addDevice(*MedHighLightingEffect);
  wemoManager.addDevice(*MedLightingEffect);
  wemoManager.addDevice(*MedLowLightingEffect);

}


void setupLights() {
  //   strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  setAll(0, 0, 0);
  delay(500);
  //  millisTrack = millis();
  strip.setBrightness(255);
  Serial.println("Lights setup");
}
//1, 2, 3, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25
void loop()
{
  //setAll(255, 250, 205);
  //return;
  if (true) {
    
    handleCase(8);
    //      handleCase(8);
    //    handleCase(8);

    handleCase(9);
    //      handleCase(9);
    //    handleCase(9);

    handleCase(10);
    //      handleCase(10);
    //    handleCase(10);

    handleCase(11);
    //      handleCase(11);
    //    handleCase(11);

    handleCase(12);
    //      handleCase(12);
    //    handleCase(12);

    handleCase(13);
    //      handleCase(13);
    //    handleCase(13);
  }
  //  wemoManager.serverLoop();
  //  Serial.println("loop");
  //Serial.println("Looping");
  oldCase = currentCase;

  handleCase(currentCase);
}



bool lightsOff() {
  currentCase = 25;
  return currentCase == 25;
}


bool rainbowLightsOn() {
  currentCase = 8;
  Serial.print("Rainbow cycle turn on ...");
  Serial.println("Setting current case to: " + String(currentCase));
  neverChanged = false;
  return currentCase == 8;
}

bool coolLightsOn() {
  currentCase = 9;
  Serial.print("Cool cycle turn on ...");
  Serial.println("Setting current case to: " + String(currentCase));
  neverChanged = false;
  return currentCase == 9;
}

bool warmLightsOn() {
  currentCase = 10;
  Serial.print("Warm cycle turn on ...");
  Serial.println("Setting current case to: " + String(currentCase));
  neverChanged = false;
  return currentCase == 10;
}

bool natureLightsOn() {
  currentCase = 11;
  Serial.print("nature cycle turn on ...");
  Serial.println("Setting current case to: " + String(currentCase));
  neverChanged = false;
  return currentCase == 11;
}

bool candyLightsOn() {
  currentCase = 12;
  Serial.print("candy cycle turn on ...");
  Serial.println("Setting current case to: " + String(currentCase));
  neverChanged = false;
  return currentCase == 12;
}

bool christmasLightsOn() {
  currentCase = 13;
  Serial.print("christmas cycle turn on ...");
  Serial.println("Setting current case to: " + String(currentCase));
  neverChanged = false;
  return currentCase == 13;
}

bool lightsOn() {
  currentCase = 24;
  Serial.print("Full White cycle turn on ...");
  Serial.println("Setting current case to: " + String(currentCase));
  neverChanged = false;
  return currentCase == 24;
}

bool mediumHighLightsOn() {
  currentCase = 26;
  Serial.print("Medium High cycle turn on ...");
  Serial.println("Setting current case to: " + String(currentCase));
  neverChanged = false;
  return currentCase == 26;
}

bool mediumLightsOn() {
  currentCase = 27;
  Serial.print("Medium cycle turn on ...");
  Serial.println("Setting current case to: " + String(currentCase));
  neverChanged = false;
  return currentCase == 27;
}

bool mediumLowLightsOn() {
  currentCase = 28;
  Serial.print("Medium Low cycle turn on ...");
  Serial.println("Setting current case to: " + String(currentCase));
  neverChanged = false;
  return currentCase == 28;
}
