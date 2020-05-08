//#include <ArduinoJson.h>

#include <ArduinoOTA.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266mDNS.h>

#define TURN_OFF_MILLIS 15*1000

//LED Stuff

#define PIN 14
int NUM_LEDS = 250;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);
bool newEffect = false;
int currentCase = 25;
int oldCase = 25;
bool neverChanged = true;

long turnOffReference = 0;
bool turnOff = false;


// prototypes
boolean connectWifi();
bool wifiConnected = false;

//on/off callbacks
//void lightOn();
//void lightOff();
void jsonCase();
void sendBodyResponse(bool success);

//------- Replace the following! ------
//char ssid[] = "UofM-Guest";       // your network SSID (name)
//char password[] = "";  // your network key
//const char* ssid = "Brady's iPhone 6";
//const char* password = "brady151";
const String ssid = "StoffelNetwork-5";
const String password = "9529471171";
const char* host = "connorlights";

//UpnpBroadcastResponder wemoManager;
String ipAddr = "";



String effectData[][29] = {{"25", "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","26", "27", "28", "29"},
                    {"Off", "Strobe", "Cylon Bounce", "Fire", "Color Wipe", "Fade in and Out", "Halloween Eyes", "KITT", "Rainbow Cycle", 
                    "Cool Cycle", "Warm Cycle", "Nature Cycle", "Candy Cycle", "Christmas Cycle", "Twinkle", "Random Twinkle", 
                    "Running Lights", "Snow Sparkle", "Bouncing Balls", "Bouncing Colored Balls", "Sparkle", "Theater Chase", 
                    "Theater Chase Rainbow", "Candy Cane", "White", "Mid", "Higher", "Dim", "Turn Off 15 Seconds" }};


int effectDataLength = 29;
ESP8266WebServer server(80);


const int ledPin = 2;

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

  ipAddr = WiFi.localIP().toString();

  //  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  //  delay(10);
  //  digitalWrite(ledPin, HIGH); // Wemos BUILTIN_LED is active Low, so high is off
}

void setupWemoStuff() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
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
//  ipAddr += ip.toString();
  Serial.println(ip);
  wifiConnected = true;

  server.on("/case", jsonCase); //Associate the handler function to the path
  server.on("/base", baseCase); //Associate the handler function to the path


  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin(host)) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  server.begin(); //Start the server
  Serial.println("Server listening");
  MDNS.addService("http", "tcp", 80);



  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
    digitalWrite(ledPin, HIGH);
  });

  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end

    for(int i = 0; i < 6; i++) {
      delay(500);
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
    }

     for(int i = 0; i < 15; i++) {
      delay(100);
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
    }
    
    
  });

  ArduinoOTA.onError([](ota_error_t error) {
    (void)error;
    ESP.restart();
  });

  /* setup the OTA server */
  ArduinoOTA.begin();
  Serial.println("Ready");
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
  oldCase = currentCase;

  handleCase(currentCase);
}


void sendBodyResponse(bool success) {

  String message = "Body received:\n";
  message += server.arg("plain");
  message += "\n";

  server.send(200, "text/plain", message);
  Serial.println(message);

}

void jsonCase() {

  String json = server.arg(0);
//  DynamicJsonDocument doc(1024);
//  deserializeJson(doc, json);
//  Serial.println(json);
//  String intString = json.substring(5);
  int doCase = json.toInt();
  Serial.print("Case: ");
  Serial.println(doCase);

  if (doCase == 29) {
    turnOffTimed();
  } else {
    turnOnCase(doCase);
  }
}

void baseCase() {
  String response = "<html>";

  response += "<head>"
"<link rel='apple-touch-icon' sizes='128x128' href='https://cdn4.iconfinder.com/data/icons/design-26/24/rgb_red_green_blue_color_monitoe_light_optical-512.png'>"
"<style>"
".button {"
"  background-color: #62B3FF;"
"  border: none;"
"  color: white;"
"  padding: 15px 32px;"
"  text-align: center;"
"  text-decoration: none;"
"  display: inline-block;"
"  font-size: 350%;"
"  margin: 4px 2px;"
"  cursor: pointer;"
"}"

".buttonRainbow {"
"  background-color: #004383;"
"}"

".buttonSelected {"
"  background-color: #a530e8;"
"}"

"body {"
"  background-color: #FF33E3;"
"}"
"</style>"
"<script>"
"function sendEffect(link, elem) {"
" elem.classList.add('buttonSelected');"
" const Http = new XMLHttpRequest();"
" const url=link;"
" Http.open('GET', url);"
" window.setTimeout('originalCols()',2000);"
" Http.send();"
"}"
"function originalCols() {"
"  var elements = document.getElementsByClassName('button');"
"  for(var i = 0; i < elements.length; i++){"
"    elements[i].classList.remove('buttonSelected');"
"  }"
"}"
"</script>"


"</head><body>";


  for (int i = 0; i < effectDataLength; i++) {
    if (effectData[0][i].equals("8")) {
      response += "<button class='button buttonRainbow' onclick='sendEffect(\"http://";
    } else {
      response += "<button class='button' onclick='sendEffect(\"http://";
    }
    
    response += host;
    response += ".local";
    response += "/case?case=";
    response += effectData[0][i];
    response += "\", this)'>";
    response += effectData[1][i];
    response += "</button>";
  }

  response += "</body></html>";
  

  server.send(200, "text/html", response);
  Serial.println(response);
}

void turnOffTimed() {
  
  Serial.print("Turning on Case: ");
  Serial.println(29);
  turnOff = true;
  turnOffReference = millis();
  neverChanged = false;
  sendBodyResponse(true);

  
}


void turnOnCase(int case1) {

  currentCase = case1;
  Serial.print("Turning on Case: ");
  Serial.println(case1);

  neverChanged = false;
  turnOff = false;
  sendBodyResponse(true);

}
