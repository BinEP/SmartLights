

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <fauxmoESP.h>
#include <Adafruit_NeoPixel.h>


//LED Stuff

#define EFFECT_NAME__0     "Strobe"
#define EFFECT_NAME__1     "Cylon Bounce"
#define EFFECT_NAME__2     "Fire"
#define EFFECT_NAME__3     "Color Wipe"
#define EFFECT_NAME__4     "Fade In and Out"
#define EFFECT_NAME__5     "Halloween Eyes"
#define EFFECT_NAME__6     "KITT"
#define EFFECT_NAME__7     "Rainbow Cycle"
#define EFFECT_NAME__8     "Cool Cycle"
#define EFFECT_NAME__9     "Warm Cycle"
#define EFFECT_NAME__10    "Nature Cycle"
#define EFFECT_NAME__11    "Candy Cycle"
#define EFFECT_NAME__12    "Christmas Cycle"
#define EFFECT_NAME__13    "Twinkle"
#define EFFECT_NAME__14    "Twinkle Random"
#define EFFECT_NAME__15    "Running Lights"
#define EFFECT_NAME__16    "Snow Sparkle"
#define EFFECT_NAME__17    "Bouncing Balls"
#define EFFECT_NAME__18    "Bouncing Colored Balls"
#define EFFECT_NAME__19    "Sparkle"
#define EFFECT_NAME__20    "Theater Chase"
#define EFFECT_NAME__21    "Theater Chase Rainbow"
#define EFFECT_NAME__22    "Candy Cane"
#define EFFECT_NAME__23    "Bright White"
#define EFFECT_NAME__24    "Off"
#define EFFECT_NAME__25    "Mid White"
#define EFFECT_NAME__26    "Upper Mid White"
#define EFFECT_NAME__27    "Low White"


#define PIN 14
int NUM_LEDS = 200;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);
bool newEffect = false;
int currentCase = 25;
int oldCase = 25;
int brightness = 255;
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

fauxmoESP fauxmo;

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
  setupWemoStuff();




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



 // By default, fauxmoESP creates it's own webserver on the defined port
    // The TCP port must be 80 for gen3 devices (default is 1901)
    // This has to be done before the call to enable()
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices

    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.enable(true);

    // You can use different ways to invoke alexa to modify the devices state:
    // "Alexa, turn yellow lamp on"
    // "Alexa, turn on yellow lamp
    // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)

    // Add virtual devices
   fauxmo.addDevice(EFFECT_NAME__0);
    fauxmo.addDevice(EFFECT_NAME__1);
    fauxmo.addDevice(EFFECT_NAME__2);
    fauxmo.addDevice(EFFECT_NAME__3);
    fauxmo.addDevice(EFFECT_NAME__4);
    fauxmo.addDevice(EFFECT_NAME__5);
    fauxmo.addDevice(EFFECT_NAME__6);
    fauxmo.addDevice(EFFECT_NAME__7);
    fauxmo.addDevice(EFFECT_NAME__8);
    fauxmo.addDevice(EFFECT_NAME__9);
    fauxmo.addDevice(EFFECT_NAME__10);
    fauxmo.addDevice(EFFECT_NAME__11);
    fauxmo.addDevice(EFFECT_NAME__12);
    fauxmo.addDevice(EFFECT_NAME__13);
    fauxmo.addDevice(EFFECT_NAME__14);
    fauxmo.addDevice(EFFECT_NAME__15);
    fauxmo.addDevice(EFFECT_NAME__16);
    fauxmo.addDevice(EFFECT_NAME__17);
    fauxmo.addDevice(EFFECT_NAME__18);
    fauxmo.addDevice(EFFECT_NAME__19);
    fauxmo.addDevice(EFFECT_NAME__20);
    fauxmo.addDevice(EFFECT_NAME__21);
    fauxmo.addDevice(EFFECT_NAME__22);
    fauxmo.addDevice(EFFECT_NAME__23);
    fauxmo.addDevice(EFFECT_NAME__24);
    fauxmo.addDevice(EFFECT_NAME__25);
    fauxmo.addDevice(EFFECT_NAME__26);
    fauxmo.addDevice(EFFECT_NAME__27);
    
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.

        if (strcmp(device_name, EFFECT_NAME__0)==0) {
            currentCase = 1;
        } else if (strcmp(device_name, EFFECT_NAME__1)==0) {
            currentCase = 2;
        } else if (strcmp(device_name, EFFECT_NAME__2)==0) {
            currentCase = 3;
        } else if (strcmp(device_name, EFFECT_NAME__3)==0) {
            currentCase = 4;
        } else if (strcmp(device_name, EFFECT_NAME__4)==0) {
            currentCase = 5;
        } else if (strcmp(device_name, EFFECT_NAME__5)==0) {
            currentCase = 6;
        } else if (strcmp(device_name, EFFECT_NAME__6)==0) {
            currentCase = 7;
        } else if (strcmp(device_name, EFFECT_NAME__7)==0) {
            currentCase = 8;
        } else if (strcmp(device_name, EFFECT_NAME__8)==0) {
            currentCase = 9;
        } else if (strcmp(device_name, EFFECT_NAME__9)==0) {
            currentCase = 10;
        } else if (strcmp(device_name, EFFECT_NAME__10)==0) {
            currentCase = 11;
        } else if (strcmp(device_name, EFFECT_NAME__11)==0) {
            currentCase = 12;
        } else if (strcmp(device_name, EFFECT_NAME__12)==0) {
            currentCase = 13;
        } else if (strcmp(device_name, EFFECT_NAME__13)==0) {
            currentCase = 14;
        } else if (strcmp(device_name, EFFECT_NAME__14)==0) {
            currentCase = 15;
        } else if (strcmp(device_name, EFFECT_NAME__15)==0) {
            currentCase = 16;
        } else if (strcmp(device_name, EFFECT_NAME__16)==0) {
            currentCase = 17;
        } else if (strcmp(device_name, EFFECT_NAME__17)==0) {
            currentCase = 18;
        } else if (strcmp(device_name, EFFECT_NAME__18)==0) {
            currentCase = 19;
        } else if (strcmp(device_name, EFFECT_NAME__19)==0) {
            currentCase = 20;
        } else if (strcmp(device_name, EFFECT_NAME__20)==0) {
            currentCase = 21;
        } else if (strcmp(device_name, EFFECT_NAME__21)==0) {
            currentCase = 22;
        } else if (strcmp(device_name, EFFECT_NAME__22)==0) {
            currentCase = 23;
        } else if (strcmp(device_name, EFFECT_NAME__23)==0) {
            currentCase = 24;
        } else if (strcmp(device_name, EFFECT_NAME__24)==0) {
            currentCase = 25;
        } else if (strcmp(device_name, EFFECT_NAME__25)==0) {
            currentCase = 26;
        } else if (strcmp(device_name, EFFECT_NAME__26)==0) {
            currentCase = 27;
        } else if (strcmp(device_name, EFFECT_NAME__27)==0) {
            currentCase = 28;
        }
        brightness = value;

    });

}


void setupLights() {
  //   strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  setAll(0, 0, 0);
  delay(500);
  //  millisTrack = millis();
  strip.setBrightness(brightness);
  Serial.println("Lights setup");
}
//1, 2, 3, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25
void loop()
{
  //setAll(255, 250, 205);
  //return;
  if (!wifiConnected) {
    
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
